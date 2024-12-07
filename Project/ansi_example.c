#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
extern void update_bpm(int bpm);

// ANSI Color Codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define CYAN        "\033[36m"
#define CLEAR_SCREEN "\033[2J"

static bool metronome_mode = false;

// Function prototypes
void main_menu();
void play();
void record();
void metronome();
void clear_screen();


// Helper function to clear the screen
void clear_screen() {
    printf(CLEAR_SCREEN);
    printf("\033[H"); // Move cursor to the top-left corner
}


void main_menu() {
    clear_screen();
    //printf("%d\n", metronome_mode);
    
    printf(BOLD CYAN "\t*** Main Menu ***\n\n" RESET);
    printf(BOLD RED "1) Play\t\t" RESET);
    printf(BOLD GREEN "2) Record\t\t" RESET);
    printf(BOLD YELLOW "3) Metronome\n\n" RESET);

    printf(BOLD CYAN "Navigation: " RESET);
    int operation;
    if (scanf("%d", &operation) != 1) {
        main_menu();
        return;
    }
    switch (operation) {
        case 1:
            play();
            break;
        case 2:
            record();
            break;
        case 3:
            metronome();
            break;
        default:
            main_menu();
    }
}

void record() {
    clear_screen();

    printf(BOLD GREEN "\t*** Record Mode ***\n\n" RESET);
    printf("Use the board to store notes. Follow instructions on the board.\n\n");
    printf(BOLD CYAN "Press Enter to return to Main Menu." RESET);
    getchar(); // Wait for Enter
    getchar();
    main_menu();
}

void play() {
    clear_screen();

    printf(BOLD RED "\t*** Play Mode ***\n\n" RESET);
    printf("Playing stored notes. Follow instructions on the board.\n\n");
    printf(BOLD CYAN "Press Enter to return to Main Menu." RESET);
    getchar(); // Wait for Enter
    getchar();
    main_menu();
}

void metronome() {
    clear_screen();
    int bpm;

    printf(BOLD YELLOW "\t*** Adjust Metronome ***\n\n" RESET);
    printf("Set your desired BPM (Beats Per Minute) or enter 3 to turn off the metronome mode.\n");
    printf("Current BPM is: %d\n", bpm);
    printf("Enter a value between 60 and 240, or 3: ");
    
    if (scanf("%d", &bpm) != 1) {
        while (getchar() != '\n'); 
        metronome();
        return;
    }


    if (bpm == 3) {
        metronome_mode = !metronome_mode; // Toggle the metronome mode
        if (metronome_mode) {
            printf(BOLD GREEN "\nMetronome turned ON.\n" RESET);
        } else {
            printf(BOLD YELLOW "\nMetronome turned OFF.\n" RESET);
        }
        printf(BOLD CYAN "Press Enter to return to Main Menu." RESET);
        getchar(); // Wait for Enter
        getchar();
        main_menu(); // Return to main menu

    }

    if (bpm < 60 || bpm > 240) {
        printf(BOLD RED "Invalid BPM. Please enter a value between 60 and 240, or 3 to exit.\n" RESET);
        while (getchar() != '\n'); // Clear input buffer
        metronome();
        return;
    }

    update_bpm(bpm); // Update BPM for the metronome
    printf(BOLD GREEN "\nMetronome set to %d BPM.\n" RESET, bpm);
    printf(BOLD CYAN "Press Enter to return to Main Menu." RESET);
    getchar(); // Wait for Enter
    getchar();
    main_menu();

}


int main() {
    main_menu();
    return 0;
}



/*
switch(mode){
    case l:
        in = 0b11000111; //L
    case p:
        in = 0b10001100; //P
}

switch(octave) {
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

}

switch(note) {
		case a:
			in = 0b10001000; //a
			break;
		case b:
			in = 0b10000011; //b
			break;
		case c:
			in = 0b11000110; //c
			break;
		case d:
			in = 0b10100001; //d
			break;
		case e:
			in = 0b10000110; //e
			break;
		case f:
			in = 0b10001110; //f
			break;
		case g:
			in = 0b10000010;; //g
			break;
}

*/