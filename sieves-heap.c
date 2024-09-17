/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>
// Skrivet av KI
#include <math.h>

#define COLUMNS 6
// Skrivet av KI
int column = 1;

void print_number(int p) {
	// Skrivet av KI
	if (COLUMNS != column++)
		printf("%10d ", p);
	else {
		printf("%10d\n", p);
		column = 1;
	}
}

void print_sieves(int n) {
    char* numbers = calloc(n - 1, sizeof(char));
    for (int i = 2; i < sqrt(n); i++)
        if (numbers[i - 2] == 0)
            for (int j = i*i; j < n+1; j += i)
                numbers[j - 2] = '1';
    for (int i = 2; i < n + 1; i++)
        if (!numbers[i - 2])
            print_number(i);
    free(numbers);
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}