#include <stdio.h>

void delay(unsigned int milli);
void pretty_print(char text[]);

//extern void print(const char*);

void pretty_print(char text[]) {
	char out;
	//char* string = {'\0', '\0'};
	char* ptr = text;
	while ((out = *(ptr++)) != '\0') {
		//*string = out;
		printf("%c", out);
		fflush(stdout);
		if (out == '.' || out == ',' || out == ':' || out == '!' || out == '?')
			delay(120);
		else
			delay(20);
	}
}

void delay(unsigned int milli) {
	unsigned int one = 1600000; // Hz/1000
	for (int i = 0; i < milli; i++) {
		for (int j = 0; j < one; j++) {
			asm volatile ("nop");
		}
	}
}


int main() {
	// printf() displays the string inside quotation
	pretty_print("Hello, World!");
	return 0;
}
