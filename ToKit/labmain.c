extern void print(const char *str);
extern void handle_interrupt();
//extern void read(char *, int);

typedef struct {
    int energy;
    int cash;
    int charisma;
    int clock;
    
    // negative effects 
    int stressed;
    int sleepy;
    int bloated;

    // positive effects
    int caffeinated;
    int productive;

    // Items
    int item1;
    int item2;
    int item3;
    int item4;

} Player;


void clear_screen();
void delay(unsigned int milli);
void pretty_print(const char text[]);
char get_choice();
void print_status(Player *player);

void morning_routine(Player *player);
void choose_outfit(Player *player);
void breakfast(Player *player);

//void car_game(Player *player); //FPGA GAME /Lose Money

void parkinglot(Player *player);
void desk(Player *player);
void colleagues(Player *player);

//void office_game(Player *player); //FPGA GAME / Lose Energy

void lunch(Player *player);
void toilet(Player *player);
void desk2(Player *player);

//void office_game2(Player *player); //FPGA GAME /Lose Something

//void clocking_out(Player *player);


// Function to initialize a player with default stats
Player create_player() {
    Player p;
    p.clock = 6;
    p.energy = 50;
    p.cash = 1000;
    p.charisma = 5;

    p.stressed = 0;
    p.sleepy = 0;
    p.caffeinated = 0;
    p.productive = 0;
    p.bloated = 0;


    p.item1 = 0;
    p.item2 = 0;
    p.item3 = 0;
    p.item4 = 0;


    return p;
}

int get_switches(int num_switches) {
    volatile int* switches = (volatile int*) 0x04000010; 
    int shift_amount = 10 - num_switches; // Shift to extract the highest `num_switches`
    
    return (*switches >> shift_amount) & ((1 << num_switches) - 1); // Mask only the needed bits
}


int get_btn( void )  {
	volatile char* button = (volatile char*) 0x040000d0;
	return *button & 1;	// Tar ut LSB som ger statur på button
}

void handle_interrupt() {}
void read(char *, int){}

// Function to clear the screen
void clear_screen() {
    print("\033[2J\033[H");
}

void pretty_print(const char text[]) {
    char out;
    char p[2];  // ✅ Small buffer to hold one character + null terminator
    const char* ptr = text;

    while ((out = *(ptr++)) != '\0') {
        p[0] = out;  // ✅ Store character in safe memory
        p[1] = '\0';  // ✅ Null-terminate the string
        print(p);  // ✅ Safe printing

        // Add delays for punctuation
        if (out == '.' || out == ',' || out == ':' || out == '!' || out == '?')
            delay(120);
        else
            delay(20);
    }
}


// Delay function for slow printing
void delay(unsigned int milli) {
    unsigned int one = 30000; 
    for (int i = 0; i < milli; i++) {
        for (int j = 0; j < one; j++) {
            asm volatile ("nop");
        }
    }
}

// Function to print player status
void print_status(Player *player) {
    print("\n--- Current Status ---\n");
    print("Energy: " + player->energy);
    print("\n");
    print("Cash: $" + player->cash);
    print("\n");
    print("Charisma: " + player->charisma);
    print("\n");
    print("Clock: " + player->clock);
    print("\n");
    
    print("Ailments: ");
    if (player->stressed) print("😵 Stressed  ");
    if (player->sleepy) print("🥱 Sleepy  ");
    if (player->caffeinated) print("☕ Over-caffeinated  ");
    if (player->productive) print("🏆 Productive  ");
    if (!player->stressed && !player->sleepy && !player->caffeinated && !player->productive) 
        print("None 😊");
    
    print("\n");
}

/*
// Function to check after every choice
void lose_status_check(Player *player) {
    if (player->energy <= 0) {
        pretty_print("You're too tired to continue... You fall asleep at your desk.\n");
        clocking_out(player);
    }
    if (player->cash <= 0) {
        pretty_print("You're broke... .\n");
        clocking_out(player);
    }
}

*/


void morning_routine(Player *player) {
    clear_screen();

    pretty_print("You wake up, but the alarm hasn't rung yet...\n");
    pretty_print("Flip the switches to choose:\n");
    pretty_print("SW8 → Snooze again\n");
    pretty_print("SW9 → Get up from bed\n");
    pretty_print("Press the button to confirm your choice...\n");

    print("  \n");
    print("  \n");
    print("  ((  (__I__)  ))\n");
    print("    .'_....._'.\n");
    print("   / / .12 . \\ \\\n");
    print("  | | '  |  ' | |\n");
    print("  | | 9  /  3 | |\n");
    print("   \\ \\ '.6.' / /\n");
    print("    '.`-...-'.'\n");
    print("    /'-- --'\\\n");
    print("    `\"\"\"\"\"\"\"\"\"`\n");

    int choice = -1;  // Initialize choice variable

    // Wait for valid input: exactly one switch must be flipped and button pressed
    while (1) {
        choice = get_switches(2);  // Read SW9-SW8
        
        if (get_btn()) {  // Confirm selection with button press
            switch (choice) {
                case 0b01: {  // SW8 ON → Snooze
                    int* randptr = (int*) 0x400042; // Adjusted memory reference
                    if ((*randptr) % 2 == 1) {  // 50% chance
                        pretty_print("You snoozed too much... You feel stressed.\n");
                        player->stressed = 1;
                    } else {
                        pretty_print("You wake up just before the alarm rings. You feel refreshed!\n");
                        player->energy += 10;
                    }
                    break;
                }

                case 0b10:  // SW9 ON → Get up immediately
                    pretty_print("Groggy and slouchy, you rise from the bed...\n");
                    player->energy -= 5;
                    break;

                default:  // Invalid selection, retry
                    pretty_print("Invalid choice! Flip only ONE switch and press the button.\n");
                    continue; // Loop again instead of calling morning_routine() recursively
            }
            break; // Exit the while loop after a valid selection
        }
    }
    pretty_print("With heavy steps, you slowly approach the wardrobe...\n");
    player->clock += 1;
    
}

    

// **Choosing an Outfit**
void choose_outfit(Player *player) {
    clear_screen();
    print_status(player);
    print("  \n");
    print("  \n");
    pretty_print("What will you wear today?\n");

    pretty_print("1. Fancy suit with fedora\n"); //Item in one of the outfits choices
    pretty_print("2. Sweatpants and sweatshirt\n");
    pretty_print("3. T-shirt and some jeans\n");

    //Print ASCII ART WARDROBE
    
   
    player->clock += 1;
    pretty_print("This seems to be office appropiate doing an agreeable head gesture\n");
    pretty_print("With newfound energy you take a stride directly to the kitchen\n");
}



// **Breakfast Choices**
void breakfast(Player *player) {
    pretty_print("What will you have for breakfast?\n");
    pretty_print("1. Coffee & toast\n");
    pretty_print("2. Breakfast francais\n");
    pretty_print("3. Leftover dinner\n");

    
    
    player->clock += 1;
    pretty_print("Checking that the oven is closed for the last time\n");
    pretty_print("You leave ur apartment and enters the car\n");


}

/*

void car_game(Player *player) {
    pretty_print("Game mode: tutorial");



    player->clock += 1;
    pretty_print("Mordor is close as you can see the office building in the horizon\n");
    parkinglot(player);

}

void parkinglot(Player *player) {
    pretty_print("You arrive at the parking lot. You see vacant spots spotted throughout\n");


    pretty_print(" "); // ASCII ART PARKING LOT

    pretty_print("1. Park besides your knobhead boss's usual spot\n");

    pretty_print("2. Park besides your bestie\n");

    pretty_print("3. Park besides your bully\n");



    player->clock += 1;
    desk(player);
}


void desk(Player *player) {
    pretty_print("You arrive at your desk. Boot up your computer as time passes...\n");

    pretty_print("1. Talk to your boss\n");

    pretty_print("2. Talk to your bestie\n");

    pretty_print("3. Talk to your bully\n");



    player->clock += 1;
    colleagues(player);
}


void colleagues(Player *player) {
    pretty_print("You see your colleagues in the distance...\n");

    pretty_print("1. Approach your boss\n");

    pretty_print("2. Approach your bestie\n");

    pretty_print("3. Approach your bully\n");

    player->clock += 1;
    //office_game(player);
}

*/


//Start
int main() {

    //Player player = create_player(); // Initialize player, create the object
    char output[20]; // Adjusted memory reference
    read(output, 20);
    print(output);
    clear_screen();
    
    //morning_routine(&player); // Start game flow
    //choose_outfit(&player); // Move to the next stage
    //breakfast(&player); // Move to the next stage

    return 0;
}
