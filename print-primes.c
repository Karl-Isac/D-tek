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

int is_prime(int n){
	// Skrivet av KI
    for (int i = 2; i <= sqrt((float) n); i++) {
      if (n % i == 0)
        return 0;
    }
    return 1;
}

void print_primes(int n){
    // Should print out all prime numbers less than 'n'
    // with the following formatting. Note that
    // the number of columns is stated in the define
    // COLUMNS

	// Skrivet av KI
    for (int i = 2; i <= n; i++) {
		if (is_prime(i) == 1)
			print_number(i);
	}
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}

 
