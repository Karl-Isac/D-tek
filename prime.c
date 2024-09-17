/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
// Skrivet av KI
#include <math.h>

int is_prime(int n){
  // Skrivet av KI
	if (n % 2 == 0)	// Enda jämna primtalet
		return 0;
    for (int i = 3; i <= sqrt((float) n); i += 2) { // Alla andra är udda
      if (n % i == 0)
        return 0;
    }
    return 1;
}
int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}
