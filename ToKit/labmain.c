extern void print(const char *str);
extern void handle_interrupt();

// 🔹 Move `Player` struct ABOVE function prototypes
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


// 🔹 Now function prototypes can use `Player`
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


    p.item1 = 0;
    p.item2 = 0;
    p.item3 = 0;
    p.item4 = 0;


    return p;
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
        pretty_print("You're broke... You can't afford to go to work.\n");
        clocking_out(player);
    }
    if (player->charisma <= 0) {
        pretty_print("You're too awkward to talk to anyone... You hide in the bathroom.\n");
        clocking_out(player);
    }
}


// Function to get player's choice
char get_choice() {
    pretty_print("\n> ");
    char choice = getchar();
    getchar(); // Consume newline
    return choice;
}

// **Morning Routine**
void morning_routine(Player *player) {
    clear_screen();
    pretty_print("You wake up, but the alarm hasn't rung yet...\n");
    pretty_print("1. Snooze again\n");
    pretty_print("2. Get up from the bed\n");
    
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


    char choice = get_choice();
    
    if (choice == '1') {
        int* randptr = 0x400042; // Anpassa efter minnesdel som ändras ofta, t.ex stacken?
        if ((*randptr)%2 == 1) { // 50% chance
            pretty_print("You snoozed too much... You feel stressed.\n");
            player->stressed = 1;
        } else {
            pretty_print("You wake up just before the alarm rings. You feel refreshed!\n");
            player->energy += 10;
        }
    } else if (choice == '2') {
        pretty_print("Groggy and slouchy, you rise from the bed...\n");
        player->energy -= 5;
    } else {
        pretty_print("Invalid choice!\n");
        morning_routine(player);
    }
    player->clock += 1;
    choose_outfit(player);
}

// **Choosing an Outfit**
void choose_outfit(Player *player) {
    clear_screen();
    print_status(player);
    print("  \n");
    print("  \n");
    pretty_print("What will you wear today?\n");
    pretty_print("1. Fancy suit with fedora\n");
    pretty_print("2. Sweatpants and sweatshirt\n");
    pretty_print("3. \n");

    //Print ASCII ART WARDROBE
    char choice = get_choice();
    
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
}



// **Breakfast Choices**
void breakfast(Player *player) {
    pretty_print("What will you have for breakfast?\n");
    pretty_print("1. Coffee & toast\n");
    pretty_print("2. Breakfast francais\n");
    pretty_print("3. Leftover dinner\n");

    char choice = get_choice();
    
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
}

void car_game(Player *player) {
    pretty_print("Game mode: tutorial");

}



//Start
int main() {

    Player player = create_player(); // Initialize player

    morning_routine(&player); // Start game flow

    return 0;
}
