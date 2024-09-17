#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6
int column = 1;
unsigned char* bit_array;

void print_number(int p) {
	// Skrivet av KI
	if (COLUMNS != column++)
		printf("%10d ", p);
	else {
		printf("%10d\n", p);
		column = 1;
	}
}

void set_bit(int index) {
    bit_array[index / 8] |= (1 << (index % 8));
}

void clear_bit(int index) {
    bit_array[index / 8] &= ~(1 << (index % 8));
}

int get_bit(int index) {
    return (bit_array[index / 8] >> (index % 8)) & 1;
}


void print_sieves(int n) {
    bit_array = calloc(n/8 + 1, 8);
    for (int i = 2; i < sqrt(n); i++)
        if (get_bit(i - 2) == 0)
            for (int j = i*i; j < n+1; j += i)
                set_bit(j - 2);
    for (int i = 2; i < n + 1; i++) {
        if (!get_bit(i - 2))
            print_number(i);
    }
    free(bit_array);
}

int main(int argc, char *argv[]) {
    if(argc == 2)
        print_sieves(atoi(argv[1]));
    else
        printf("Please state an integer number.\n");
    printf("\n");
    return 0;
}
