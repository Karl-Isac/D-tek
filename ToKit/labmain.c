extern void print(const char *str);
extern void handle_interrupt();

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

void car_game(Player *player); //FPGA GAME /Lose Money

void parkinglot(Player *player);
void desk(Player *player);
void colleagues(Player *player);

void office_game(Player *player); //FPGA GAME / Lose Energy

void lunch(Player *player);
void toilet(Player *player);
void desk2(Player *player);

void office_game2(Player *player); //FPGA GAME /Lose Something

void clocking_out(Player *player);


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


    p.keycard = 0;
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


// Function to clear the screen
void clear_screen() {
    print("\033[2J\033[H");
}

// Pretty Print Function: Creates slow typewriter effect
void pretty_print(const char text[]) {
    char out;
    const char* ptr = text;
    while ((out = *(ptr++)) != '\0') {
        char* p;
        *p = out;
        *(p+1) = '\0';
        print(p);
        if (out == '.' || out == ',' || out == ':' || out == '!' || out == '?')
            delay(120);
        else
            delay(20);
    }
}

// Delay function for slow printing
void delay(unsigned int milli) {
    unsigned int one = 1600000; 
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
    if (player->charisma <= 0) {
        pretty_print("You're too awkward to talk to anyone... You hide in the bathroom.\n");
        clocking_out(player);
    }
}




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
    
        // Wait for the player to select a switch and press the button
        while (choice == -1) {
            choice = get_switches(2);  // Read SW9-SW8
            if (get_btn()) {  // Confirm with button press
                break;
            }
        }
    
        if (choice == 0b01) {  // SW8 ON → Snooze
            int* randptr = (int*) 0x400042; // Adjusted memory reference
            if ((*randptr) % 2 == 1) {  // 50% chance
                pretty_print("You snoozed too much... You feel stressed.\n");
                player->stressed = 1;
            } else {
                pretty_print("You wake up just before the alarm rings. You feel refreshed!\n");
                player->energy += 10;
            }
        } else if (choice == 0b10) {  // SW9 ON → Get up immediately
            pretty_print("Groggy and slouchy, you rise from the bed...\n");
            player->energy -= 5;

        } else {  // Invalid selection, retry
            pretty_print("Invalid choice!\n");
            morning_routine(player);
            return;
        }
    
        player->clock += 1;
        choose_outfit(player); // Move to the next stage
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
    pretty_print("3. \n");

    //Print ASCII ART WARDROBE
    
    if (choice == '1') {
        pretty_print("You look sharp! People will respect you more today.\n");
        player->charisma += 5;
        player->cash -= 50;
    } else if (choice == '2') {
        pretty_print("You look casual and comfortable.\n");

    } else if (choice == '3') {
        pretty_print("You look like you just woke up... Maybe people will judge you.\n");
        player->charisma -= 5;
    } else {
        pretty_print("Invalid choice!\n");
    }
    player->clock += 1;
    breakfast(player);
    pretty_print("This seems to be office appropiate doing an agreeable head gesture\n");
    pretty_print("With newfound energy you take a stride directly to the kitchen\n");
}



// **Breakfast Choices**
void breakfast(Player *player) {
    pretty_print("What will you have for breakfast?\n");
    pretty_print("1. Coffee & toast\n");
    pretty_print("2. Breakfast francais\n");
    pretty_print("3. Leftover dinner\n");

    
    if (choice == '1') {
        pretty_print("A classic breakfast! You feel a bit more awake.\n");
        player->energy += 10;
        player->cash -=20;
    } else if (choice == '2') {
        pretty_print("You go out and light a cigarette...\n");
        

    } else if (choice == '3') {
        pretty_print("That was delicious, but you stomach start to feel tight...\n");
        player->energy += 20;
        player->cash -=40;
        player->bloated += 1; //This will affect later, farting during colleagues scene
    } else {
        pretty_print("Invalid choice!\n");
    }
    player->clock += 1;
    car_game(player);
    pretty_print("Checking that the oven is closed for the last time\n");
    pretty_print("You leave ur apartment and enters the car\n");


}

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
    office_game(player);
}



//Start
int main() {

    Player player = create_player(); // Initialize player, create the object

    morning_routine(&player); // Start game flow

    return 0;
}
