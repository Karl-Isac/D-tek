/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) {
}

/* Add your code here for initializing interrupts. */
void labinit(void) { 
}

void set_leds(int led_mask) {
	volatile short* LED = (volatile short*) 0x04000000; // - 0x0400000f = 16 bits = short
	*LED = led_mask;
}

void set_displays(int display_number, int value) {
	volatile char* display = (volatile char*) 0x4000050;
	display += 0x10 * display_number;	// om dn = 0, ingen offset
	int in = 0b11111111;	// Allt av

	switch(value) { // MSB är 1 för att stänga av dp (decimal point)
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
	return *switches & 0x3ff; // maska med 10 1:or
}

int get_btn( void )  {
	volatile char* button = (volatile char*) 0x040000d0;
	return *button & 1;	// Tar ut LSB som ger statur på button
}

void update_display(int hours, int minutes, int seconds) {
	set_displays(5, hours/10);
	set_displays(4,hours%10);
	set_displays(3, minutes/10);
	set_displays(2,minutes%10);
	set_displays(1,seconds/10);
	set_displays(0,seconds%10);
}

int main() {
  labinit();
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  int not_done = 1; // 1 = true, 0 = false

  while (1) {
    time2string( textstring, mytime );
    display_string( textstring );
    delay( 2 );
	// Jämför värdet på sekunder med 16, SEN plus 1
	if (seconds++ < 16 && not_done) {
		set_leds(seconds);
		if (seconds == 15)
			not_done = 0;
		}

	if (seconds == 60){
			seconds = 0;
			minutes++;
			if (minutes == 60) {
				minutes = 0;
				hours++;
				if (hours == 100) {
					hours = 0;
				}
			}
		}

	if (get_btn()) {	// get_btn() returnerar 1 eller 0
		int state = get_sw();
		int dis = state >> 8;
		switch(dis) {
			case 1:				// 63 = 0x3f = 0b111111 (6 st 1:or)
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
	update_display(hours, minutes, seconds);

    tick( &mytime );
  }
}


