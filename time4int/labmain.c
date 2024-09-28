/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */

extern void print(const char*);
extern void print_dec(unsigned int);
extern void tick(int*);
extern int nextprime( int );
extern void enable_interrupt( void );
// Dessa finns sen men är här för att deklareras i förväg
void set_leds(int led_mask);
void set_displays(int display_number, int value);
void update_display(int hours, int minutes, int seconds);
int get_sw( void );


int prime = 1234567;
int hours = 0;
int minutes = 0;
int seconds = 0;
int not_done = 1;

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

int timeoutcount;

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt ( unsigned cause ) {
	volatile unsigned short* timer = (volatile unsigned short*) 0x04000020;
	*timer = 0;	// Återställer hela status registret, påverkar inte RUN-biten
	if (++timeoutcount == 10) {
		timeoutcount = 0;
		counter_stuff();
		tick( &mytime );
		update_display(hours, minutes, seconds);
	}
}

void counter_stuff( void ) {
	if (not_done && seconds++ < 16) {
		set_leds(seconds);
		if (seconds == 15)
			not_done = 0;
		}

	if (seconds >= 60){
			seconds = 0;
			minutes++;
			if (minutes >= 60) {
				minutes = 0;
				hours++;
				if (hours >= 100) {
					hours = 0;
				}
			}
		}
}

void button_stuff( void ) {
	int state = get_sw();
	int dis = state >> 8;
	switch(dis) {
		case 1:
			seconds = state & 63;
			break;
		case 2:
			minutes = state & 63;
			break;
		case 3:
			hours = state & 63;
			break;
	}
}

/* Add your code here for initializing interrupts. */
void labinit(void) {
	// 3*10⁶ sek == 0000 0000 0010 1101 : 1100 0110 1100 0000 - 1 sek
	volatile unsigned short* timer = (volatile unsigned short*) 0x04000020;
	*(timer + 2*2) = (29999999/10) & 0xffff;	// Fixa med unsigned short istället
	*(timer + 3*2) = (29999999/10) >> 16;
	*(timer + 1*2) = 7;
	enable_interrupt();
}

void set_leds(int led_mask) {
	volatile char* LED = (volatile char*) 0x04000000;
	*LED = led_mask;
}

void set_displays(int display_number, int value) {
	volatile char* display = (volatile char*) 0x4000050;
	display += 0x10 * display_number;
	int in = 0b11111111;

	switch(value) {
		case 0:
			in = 0b11000000;
			break;
		case 1:
			in = 0b11111001;
			break;
		case 2:
			in = 0b10100100;
			break;
		case 3:
			in = 0b10110000;
			break;
		case 4:
			in = 0b10011001;
			break;
		case 5:
			in = 0b10010010;
			break;
		case 6:
			in = 0b10000010;
			break;
		case 7:
			in = 0b11111000;
			break;
		case 8:
			in = 0b10000000;
			break;
		case 9:
			in = 0b10011000;
			break; 
	}
	*display = in;
}

int get_sw( void )  {
	volatile int* switches = (volatile int*) 0x04000010;
	return *switches & 0x3ff;
}

int get_btn( void )  {
	volatile int* button = (volatile int*) 0x040000d0;
	return *button & 1;
}

void update_display(int hours, int minutes, int seconds) {
	set_displays(5, hours/10);
	set_displays(4, hours%10);
	set_displays(3, minutes/10);
	set_displays(2, minutes%10);
	set_displays(1, seconds/10);
	set_displays(0, seconds%10);
}

int main ( void ) {
	labinit();
	while (1) {
		print ("Prime: ");
		prime = nextprime( prime );
		print_dec( prime );
		print("\n");
	}
}
