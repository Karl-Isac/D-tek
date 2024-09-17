/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";


void printlist(const int* lst){
	printf("ASCII codes and corresponding characters.\n");
	while(*lst != 0){
		printf("0x%03X '%c' ", *lst, (char)*lst);
		lst++;
	}
	printf("\n");
}

void endian_proof(const char* c){
	printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}
// Skriven av KI
void copycodes(char* text, int* list, int* counter) {
	if (*text != 0) {
		*list = *text;
		text++;
		list++;
		*counter += 1;	// *counter++; funkar inte?
		copycodes(text, list, counter);
	}
}
// Skriven av KI
void work(int* list1, int* list2, int* counter) {
	copycodes(text1, list1, counter);
	copycodes(text2, list2, counter);
}

int main(void){
	// Skriven av KI
    int* list1 = malloc(80);
    int* list2 = malloc(80);
	int counter = 0;

    work(list1, list2, &counter);

    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", counter);
	// Skriven av KI
	free(list1);
	free(list2);

    endian_proof((char*) &counter);
}
