#include "labmain.h"
#include "coffee.h"

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

    if (num_switches > 0) {
        return (*switches) & ((1 << num_switches) - 1);  // Mask the lowest `num_switches` bits
    }
    return (*switches); // Return all switches if num_switches is 0
}

int get_btn( void )  {
	volatile char* button = (volatile char*) 0x040000d0;
	return *button & 1;	// Tar ut LSB som ger statur på button
}

void handle_interrupt ( unsigned cause ) {
    if (coffee == 0) {
        print("You are out of coffee!!!\n");
        return;
    }
    coffee--;
    c_energy = 10;
    display_energy();
    set_coffee();
}


void clear_screen() {
    print("\033[2J\033[H");
}

void clear_row() {
    print("\033[1A\033[2K");
}

void error_message() {
    print("Invalid choice! Flip only ONE switch and press the button.\n");
    clear_row();
}

void set_leds(int led_mask) {
	volatile char* LED = (volatile char*) 0x04000000;
	*LED = led_mask;
}

void pretty_print(const char text[]) {
    char out;
    char p[2];  //  Small buffer to hold one character + null terminator
    const char* ptr = text;

    while ((out = *(ptr++)) != '\0') {
        p[0] = out;  //  Store character in safe memory
        p[1] = '\0';  
        print(p);  

        // Add delays for punctuation
        if (out == '.' || out == ',' || out == ':' || out == '!' || out == '?')
            delay(120);
        else
            delay(20);
    }
}


// Delay function for slow printing
void delay(unsigned int milli) {
    unsigned int one = 3000; 
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

void scene_init(Player *player) {

    if (player->energy <= 0) {
        game_over(player);
    }
    if (player->cash <= 0) {
        game_over(player);
    }
    clear_screen();
    print_status(player);
    print("  \n");
    print("  \n");
}

void timer_init(void) {
    volatile unsigned short* timer = (volatile unsigned short*) 0x04000020;
    *(timer + 2*2) = (29999999/10) & 0xffff;
    *(timer + 3*2) = (29999999/10) >> 16;
    *(timer + 1*2) = 6;	// 0b0111; enable: start, cont, NOT ITO (Interrupt TiomOut)
}

int get_random_bit() {
    volatile int* snapl = (volatile int*) (0x04000020 + 4);  
    int lower = *snapl;   // Read lower 16-bits
    return lower & 1;  // Combine into full 32-bit value
}



void close_display() {
	volatile char* display = (volatile char*) 0x4000050;
    *display = 0b11111111;
    for (int i = 1; i < 6; i++) {
        display += 0x10;
        *display = 0b11111111;
    }
}

void morning_routine(Player *player) {
    scene_init(player);

    pretty_print("You wake up, but the alarm hasn't rung yet...\n");
    pretty_print("Flip the switches to choose:\n");
    pretty_print("SW0 → Snooze again\n");
    pretty_print("SW1 → Get up from bed\n");
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

    int choice;  // Initialize choice variable
    while (1) {
        choice = get_switches(2);  // Read SW0-SW1
        set_leds(choice);  // Light up the LEDs according to the switches
        
        if (get_btn()) {  // Confirm selection with button press
            switch (choice) {
                case 0b01: {  // SW0 ON → Snooze
                    //int* randptr = (int*) 0x400042; // Adjusted memory reference
                    if (1) {  // 50% chance
                        pretty_print("You snoozed too much... You feel stressed.\n");
                        player->stressed = 1;
                    } else {
                        pretty_print("You wake up just before the alarm rings. You feel refreshed!\n");
                        player->energy += 10;
                    }
                    break;
                }

                case 0b10:  // SW1 ON → Get up immediately
                    pretty_print("Groggy and slouchy, you rise from the bed...\n");
                    player->energy -= 5;
                    break;

                default:  // Invalid selection, retry
                    error_message();
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
    scene_init(player);
    
    pretty_print("Fancying yourself, what will you wear today?\n");
    pretty_print("1. Fancy suit with fedora\n"); //Item in one of the outfits choices
    pretty_print("2. Sweatpants and sweatshirt\n");
    pretty_print("3. T-shirt and some jeans\n");

    print("        _________________________\n");
    print("       (, ______________________ )\n");
    print("       | |                      ||\n");
    print("       | |          @@@@        ||            @@@@\n");
    print("       | |        @@@@@@@       ||          @@@@@@@\n");
    print("       | |         @@ - -       ||          - @@@@\n");
    print("       | |          @  c/       ||         '_ @@@\n");
    print("       | |        _@| |_        ||         __\\@ \\@\n");
    print("       | |       ( \\ )/_\\ /_  ||  _\\\\  (/ ) @\\_/)\n");
    print("       | |        \\ \\|) / \\) ||   |(__/ /     /|\n");
    print("       | |        |\\_/ ( -/    ||    \\___/ ----/_|\n");


    int choice;  // Initialize choice variable

    while(1){
        choice = get_switches(3);  // Read SW9-SW8
        set_leds(choice);  // Light up the LEDs according to the switches

        if (get_btn()) {  // Confirm selection with button press
            switch (choice) {
                case 0b001:  // SW8 ON → Fancy suit
                    pretty_print("You look dapper in your suit and fedora!\n");
                    player->charisma += 5;
                    player->cash -= 100;
                    player->item1 = 1;
                    break;

                case 0b010:  // SW9 ON → Sweatpants
                    pretty_print("You feel comfy in your sweatpants and sweatshirt.\n");
                    player->energy += 5;
                    player->charisma -= 3;
                    player->cash += 50;
                    player->item2 = 1;
                    break;

                case 0b100:  // SW10 ON → T-shirt
                    pretty_print("You're casual in your t-shirt and jeans.\n");
                    player->energy += 3;
                    player->charisma += 1;
                    player->cash += 20;
                    player->item3 = 1;
                    break;    

                default:  // Invalid selection, retry
                    pretty_print("Invalid choice! Flip only ONE switch and press the button.\n");
                    clear_row();
                    continue; // Loop again instead of calling choose_outfit() recursively
            }
            break; // Exit the while loop after a valid selection
        }
    }    
   
    player->clock += 1;
    pretty_print("This seems to be office appropiate doing an agreeable head gesture\n");
    pretty_print("With newfound energy you take a stride directly to the kitchen\n");
}



// **Breakfast Choices**
void breakfast(Player *player) {
    clear_screen();
    print_status(player);
    print("  \n");
    print("  \n");

    pretty_print("What will you have for breakfast?\n");
    pretty_print("1. Coffee & toast\n");
    pretty_print("2. Breakfast francais\n");
    pretty_print("3. Leftover dinner\n");

    print("⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡃⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⣇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");

    
    int choice;  // Initialize choice variable
    while (1){
        choice = get_switches(3);  // Read SW10-SW8
        set_leds(choice);  // Light up the LEDs according to the switches

    if (get_btn()) {  // Confirm selection with button press
        switch (choice) {
            case 0b001:  // SW8 ON → Coffee & toast
                pretty_print("You feel energized after your coffee and toast!\n");
                player->energy += 10;
                player->cash -= 5;
                player->caffeinated = 1;
                break;

            case 0b010:  // SW9 ON → Breakfast francais
                pretty_print("You go out the balcony and light up a ciggarete!\n");
                player->energy += 5;
                player->cash -= 10;
                player->charisma += 2;
                break;
            
            case 0b100:  // SW10 ON → Leftover dinner
                pretty_print("You feel full after your leftover dinner!\n");
                player->energy += 3;
                player->cash += 0;
                player->bloated = 1;
                break;
            
            default:  // Invalid selection, retry
                pretty_print("Invalid choice! Flip only ONE switch and press the button.\n");
                continue; // Loop again instead of calling breakfast() recursively
            }
            break; // Exit the while loop after a valid selection
        }

    player->clock += 1;
    pretty_print("Checking that the oven is closed for the last time\n");
    pretty_print("You hurry down the steps of ur apartment and enters the car\n");
}
}

///////////CAR GAME HERE////////////

void draw_road(int progress, int* krockar, int player_lane) {
    clear_screen();
    for (int i = ROAD_HEIGHT + progress - 1; i > progress; i--) {
        print("| ");
        char out = (enemy_lanes[i*3] == '0') ? ' ' : enemy_lanes[i];
        print(out);
        print(" | ");
        out = (enemy_lanes[i*3 + 1] == '0') ? ' ' : enemy_lanes[i];
        print(out);
        print(" | ");
        out = (enemy_lanes[i*3 + 2] == '0') ? ' ' : enemy_lanes[i];
        print(out);
        print(" |");
        print("\n");
    }
    print("| ");
    for (int i = progress*3; i < progress*3 + 3; i++) {
        if (i == player_lane) {
            print("A");
            if (enemy_lanes[i] != '0') (*krockar)++;
        } else {
            char out = (enemy_lanes[i] == '0') ? ' ' : enemy_lanes[i];
            print(out);
        }
        print(" | ");
    }
}

int get_switches_event() {
    volatile int* switches = (volatile int*) 0x04000010;
    int ret =  *(switches + 3);
    *(switches + 3) = 0;
    if (ret & 1) return 2;
    if (ret & (1 << 9)) return 1;
    return 0;
}

void display_krockar(int krockar) {
    int foo = 1;
    if (krockar >= 10) foo = 1023;
    else {
        for (int i = 1; i <= krockar; i++)
            foo *= 2;
        foo -= 1;
    }
    set_leds(foo);
}

void car_game(Player *player) {
    pretty_print("Time to drive to work\nFlip SWI9 to turn left and SWI0 to turn right.\nAvoid the cars.");
    delay(300);
    clear_screen();
    int dir;
    int player_lane = 1; // Start in the middle lane
    int progress = 0;
    int krockar = 0;
    while (progress < 100) {
        delay(150);
        if ((dir = get_switches_event()) != 0) {
            if (dir == 1 && player_lane > 0) player_lane--;
             else if (player_lane < NUM_LANES - 1) player_lane++;
        }
        draw_road(progress, &krockar, player_lane);
        progress++;
        display_krockar(krockar);
    }
    if (krockar != 0) {
        pretty_print("You crashed ");
        print_dec(krockar);
        pretty_print(" times and each crash costed 100 currency\nHope you had enough :/");
        player->cash -= 100 * krockar;
        if (player->cash < 0) game_over(player);
    }
}
/////////////////////////////////////
void parkinglot(Player *player) {
    clear_screen();
    print_status(player);
    print("  \n");
    print("  \n");
    pretty_print("You arrive at the parking lot. You see vacant spots spotted throughout\n");

    print("  \n");
    print("  \n");

    print("      ________________________\n");
    print("     /////////////|\\\\\\\\\\\\n");
    print("    '.-------------------------.'\n");
    print("     |                         |\n");
    print("     | [] [] [] [] [] [] [] [] |\n");
    print("     |                         |\n");
    print("   _.-.        _ _2_ _         |\n");
    print("   >   )] [] []||_||||[] [] [] |\n");
    print("   `.,'________||___||_________|  <\n");
    print("    ||  /  _<> _     _    (_)<> |\n");
    print("    '' /<>(_),:/     \\:. <>'  <>|\n");
    print("    __::::::::/ _ _ _ \\:::::::::::_\n");


    print("  \n");

    pretty_print("1. Park besides your knobhead boss's usual spot\n");
    pretty_print("2. Park besides your bestie\n");
    pretty_print("3. Park besides your nemesis\n");


    int choice;
    while (1){
        choice = get_switches(3);
        set_leds(choice);
    
    if (get_btn()){
        switch (choice)
        {
        case 0b001:
            /* code */
            break;
        
        case 0b010:
            /* code */
            break;
        case 0b100:
            /* code */
            break;

        default:  
            error_message();
            continue; // Loop again instead of calling breakfast() recursively
        }
        break; // Exit the while loop after a valid selection
    }

    player->clock += 1;
    pretty_print("You enter the building and waved at the receptionist\n");
}
}

void desk(Player *player) {

    if (player->bloated) {
        clear_row();
        pretty_print("Maybe i shouldn't have eaten that leftover dinner...\n");
        clear_row();
        pretty_print("You farted loudly and everyone turned to look at you...\n");
        clear_row();
        pretty_print("You feel embarrassed.\n");
        player->charisma -= 10;
    }

    scene_init(player);

    print("  \n");
    print("  \n");
    pretty_print("You arrive at your desk. Boot up your computer as time passes...\n");

    print("::::==========:::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    print(":::::::=========::::::.---------------.:::::::::::::::::::::::::::::::\n");
    print(":::=============::::::| .-----------. |:::::::::::::::::::::::::::::\n");
    print("::::==========::::::::| | === == == | |:::::::::::::::::::::::::::::::::\n");
    print("::::==========::::::::| | EMAIL  DO | |:::::::::::::::::::::::::::::::\n");
    print(":::::::=========='::::| |  urgent!  | |:::::::::::::::::::::::::::::\n");
    print(":::===========::::::::| |___________| |::::::((;):::::::::::::::::::::\n");
    print("''''============''''''|___________oo__|\"\")'''';(\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
    print("  ==========='           ___)___(___,o  (   .---._\n");
    print("     ===========        |___________| 8  \\ |TEA|_)    .+-------+.\n");
    print("  ===========                     o8o8    ) |___|    .' |_______| `.\n");
    print("_______________________________________________________________________\n");

    print("  \n");
    print("  \n");
    pretty_print("1. Talk to your boss\n");
    pretty_print("2. Talk to your bestie\n");
    pretty_print("3. Talk to your neighbour\n");

    int choice;
    while (1){
        choice = get_switches(3);
        set_leds(choice);
        if (get_btn()){
            switch (choice)
            {
            case 0b001: //Find keycard from parking lot, get cash

                /* code */
                break;
            
            case 0b010:
            pretty_print("Nerd things\n"); //Find toy from parking lot, get charisma
                /* code */
                break;

            case 0b100:
                /* code */
                break;
            
            default:  // Invalid selection, retry
                error_message();
                continue; 
            }
            break; // Exit the while loop after a valid selection
        }

    player->clock += 1;
}
}

///////////COFFEE GAME HERE////////////

void display_energy() {
    int foo = 1;
    if (c_energy >= 10) foo = 1023;
    else {
        for (int i = 1; i <= c_energy; i++)
            foo *= 2;
        foo -= 1;
    }
    set_leds(foo);
}

void print_sak(const char* bild[]) {
    clear_screen();
    int i = 0;
    while(meeting[i] != 0)
        print(bild[i++]);
}

void set_coffee() {
    close_display();
    volatile char* display = (volatile char*) 0x4000050;
    if (coffee == 1) {
        *display = 0b11000000;
        return;
    }
    *display = 0b11110000;
    int i = 2;
    while (i < coffee) {
        display += 0x10;
        *display = 0b11110110;
    }
    display += 0x10;
    *display = 0b11000110;
}

void coffee_game(Player *player) {
    __asm__ volatile (
        "csrsi mie, 18\n\t"      // Set bit 18 in the mie register (enable interrupt)
        "csrsi mstatus, 3\n\t"   // Set bits 0 and 1 in mstatus (enable global interrupts)
    );
    pretty_print("It is time for a meeting.      \nFuck          \n\nThis will seriosly drain your energy, but you have a cup of coffee to keep up you energy :)\n");
    set_coffee();
    pretty_print("Press BTN to take a sip of coffee.        \nBut don't drink too fast, you only have one cup for the entire meeting.\n");
    display_energy();
    pretty_print("You can see how much energy you have on the LEDs");
    delay(300);
    print_sak(meeting);
    delay(700);
    for (int i = 0; i < 15; i++) {
        print_sak(boss3);
        delay(100);
        print_sak(boss0);
        delay(100);
        print_sak(boss1);
        delay(100);
        print_sak(boss2);
        c_energy--;
        display_energy();
        delay(150);
    }
    print_sak(funny);
    pretty_print("Your favorite colleague has shared a funny anecdote.          \n");
    c_energy = (c_energy < 10) ? c_energy + 3 : 10;
    pretty_print("Thank god!!! You gain some energy back.    ");
    display_energy();
    for (int i = 0; i < 12; i++) {
        print_sak(boss3);
        print("Back to buisness...");
        delay(100);
        print_sak(boss0);
        print("Back to buisness...");
        delay(100);
        print_sak(boss1);
        print("Back to buisness...");
        delay(100);
        print_sak(boss2);
        print("Back to buisness...");
        c_energy--;
        display_energy();
        delay(150);
    }

    print_sak(boring);
    pretty_print("EEUGHH!!! The colleague you don't like is telling a SUPER BORING pointless story\nYou loose half of your energy (rounded down)!\n");
    c_energy /= 2;
    display_energy();

    print_sak(meeting);
    pretty_print("Your boss somehow became extra boring\nYou now loose energy faster\n");
    delay(150);
    for (int i = 0; i < 12; i++) {
        print_sak(boss3);
        print("Back to buisness...");
        delay(100);
        print_sak(boss0);
        print("Back to buisness...");
        delay(100);
        c_energy--;
        display_energy();
        print_sak(boss1);
        print("Back to buisness...");
        delay(100);
        print_sak(boss2);
        print("Back to buisness...");
        c_energy--;
        display_energy();
        delay(150);
    }
    __asm__ volatile (
        "csrsi mstatus, 0\n\t"   // de-Set bits 0 and 1 in mstatus (enable global interrupts)
    );
    print_sak(question);
    delay(50);
    pretty_print("You just got a question...\nHurry up and answer!\nYou have 3 seconds to switch SW4 to answer correctly\n");
    pretty_print("3... ");
    delay(600); // 1000 - (3*120 + 20*2)
    pretty_print("2... ");
    delay(600);
    pretty_print("1... ");
    delay(600);
    int res = get_switches(0) & (1 << 4);
    if (res) {
        print_sak(question_good);
        pretty_print("Good job :)\nYou gained +2 charisma.");
        player->charisma += 2;
    } else {
        print_sak(question_bad);
        pretty_print("PAY ATTENTION");
        print("*player_name*");
        pretty_print("!!!\nThis is very bad for your image. -10 charisma!");
        player->charisma -= 10;
    }
    delay(100);
    clear_screen();
    pretty_print("The meeting is now over. *Phew*\nBut you lost some energy...");
    player->energy += c_energy - 10;
}



void lunch(Player *player) {
    scene_init(player);

    pretty_print("*Stomach growling* ... I guess I need to feed the beast?\n");
    clear_row();
    pretty_print("You head to the cafeteria\n");
    print("  \n");
    print("  \n");

    pretty_print("1. Salad\n");
    pretty_print("2. Burger\n");
    pretty_print("3. Sushi\n");

    int choice;
    while (1){
        choice = get_switches(3);
        set_leds(choice);
        if (get_btn()){
            switch (choice)
            {
            case 0b001:
                /* code */
                break;
            
            case 0b010:
                /* code */
                break;
            
            case 0b100:
                /* code */
                break;
            
            default:  // Invalid selection, retry
                error_message();
                continue; 
            }
            break; // Exit the while loop after a valid selection
        }
    player->clock += 1;
}
}

void toilet(Player *player) {
    scene_init(player);

    pretty_print("You feel the urge to go to the toilet.\n");
  
    pretty_print("1. Go to the toilet\n");
    pretty_print("2. Hold it in\n");

    int choice;
    while (1){
        choice = get_switches(2);
        set_leds(choice);
        if (get_btn()){
            switch (choice)
            {
            case 0b01:
                /* code */
                break;
            
            case 0b10:
                /* code */
                break;
            
            default:  // Invalid selection, retry
                error_message();
                continue; 
            }
            break; // Exit the while loop after a valid selection
        }
    player->clock += 1;
}
}

///////////TERMINAL GAME HERE////////////

void clocking_out(Player *player) {
    clear_screen();

    pretty_print("You're finally done for the day. You clock out and head home...\n");
    pretty_print("You survived another day at the office!\n");
    pretty_print("You raise your fists in the air in triumph!\n");

    print("  \n");
    print("  \n");


    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡾⢛⢻⣷⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⢀⣾⣟⠛⢿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣿⠋⠰⡀⢎⣿⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⢸⣿⠀⠎⠠⡙⣿⡄⠀⠀⢀⣀⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⡿⣷⣦⣴⣏⣽⡟⠡⠌⡱⠈⡼⣿⣀⡀⠀⠀⠀\n");
    print("⠀⢀⣴⡾⢿⣌⠌⡁⢆⠹⣿⣄⣠⣿⠛⠯⣝⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣇⠐⡈⢿⣿⣿⡟⠁⢆⠱⢀⡱⡝⢉⡉⢿⣦⠀⠀\n");
    print("⢀⣿⠇⡐⡈⠻⣦⠁⢂⠆⠻⣿⣿⡃⢉⠔⣫⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⢿⣎⠰⢈⠙⡋⢅⠊⡄⠢⡁⡑⢠⠂⠔⣈⣿⠆⠀\n");
    print("⠐⣿⡜⢠⠐⡡⢠⠘⠤⡈⠔⣈⢉⠰⠈⡜⣼⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣷⡆⠡⠒⡌⠰⣀⠣⠐⢌⢂⠩⡐⢬⠿⣦⡄\n");
    print("⠀⢙⣿⣷⠆⡑⢠⠉⡐⢡⠊⠔⢂⠙⣲⣼⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣷⣥⠁⢆⠱⠀⢆⠩⠄⢊⠰⠐⣂⠰⢘⣿\n");
    print("⠘⣿⡏⢀⠢⡑⢂⠡⡘⢠⠑⡌⢂⠒⣼⡟⠀⠀⠀⣀⣤⣤⣶⣶⣶⣶⣶⣦⣴⣤⣤⣄⣀⠈⠻⣷⣤⡂⢉⠢⠘⡈⠆⠡⠃⢄⠒⣼⡿\n");
    print("⠀⠹⣷⣌⠄⡑⡈⠆⡑⢂⠢⠐⣬⣾⠟⢁⣤⣾⠿⠛⢋⢹⣷⠖⡠⢷⣆⡐⡈⢉⢉⡙⠛⠿⢷⣮⣝⠻⣷⣶⣥⣦⣬⣥⣼⣤⣾⠟⠁\n");
    print("⠀⠀⠈⠙⠻⢷⣌⣄⢡⣢⣴⣿⠟⢉⣴⡟⠋⠄⣂⣡⣾⡿⢋⠰⠀⢭⡻⢷⣷⣬⣤⣀⣃⣐⠂⡉⠛⢷⣦⠈⠉⠉⠉⠉⠛⠋⠁⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠙⠛⠛⠛⠉⢀⣴⡟⠳⠶⢷⠾⠟⠫⣵⣯⡔⢎⡱⢾⣷⣤⡈⢉⠍⠛⠛⢋⡐⠡⢘⡀⠛⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⠋⠤⠡⠌⡄⠢⣬⣿⣿⣏⡜⣪⡑⢯⣿⣿⣿⣦⢈⠢⢑⠢⡘⢄⠃⠤⠑⡈⢿⣦⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡿⠁⢎⡐⠡⢊⣤⣷⣿⣿⣻⠟⡭⠵⢭⡋⠿⣽⣿⣿⣷⣦⣆⣂⣁⠢⠘⢠⠃⠌⢊⢿⡆⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⠃⡍⠰⣰⣿⣿⣿⠿⣿⢶⠏⡟⢬⡙⢦⡍⢧⡙⢷⣽⣻⡿⢿⣿⣿⣿⣇⠂⠜⡈⠔⣺⣿⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣼⡟⢰⡈⠌⣿⡟⡭⢎⡽⣿⠀⠛⡬⢣⡙⣆⠞⡦⠙⠆⢌⢹⣯⡓⢶⡘⣾⣯⠀⢣⠘⡠⢹⣿⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⢺⡐⡈⣿⡽⣘⢧⣚⣿⠀⢍⣰⣦⣷⣾⣶⣶⣭⡐⢨⠸⣷⡹⢲⡹⢼⣷⠈⢄⠣⢐⣩⣿⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡣⠥⡃⢌⣿⡵⣩⠶⣩⣿⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⡘⣿⡱⣏⡜⣻⣯⠐⣈⠒⡄⢺⣿⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠒⡅⠢⣿⢖⡭⢳⡱⣯⣿⣿⡿⣟⣯⢟⣽⣻⣿⣿⣿⣿⡽⣱⠞⡼⣹⣷⠐⢠⢃⢞⣽⠇⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⢿⣯⠁⡆⠡⣿⠞⣜⢧⡹⣾⣿⣿⣿⠿⠿⠿⢷⣷⣿⣿⣿⣿⣧⠹⣞⡱⢣⣿⠀⡡⢎⣾⠏⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣇⡘⠠⣿⡛⡼⢢⣓⣿⠟⡉⠄⣂⠆⢒⠠⡀⠌⡙⠛⣛⣮⣓⠮⣕⣻⡏⡰⣴⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣦⡄⠻⣿⡇⢣⡿⠃⢠⠃                                       \n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣦⡄⠻⣿⡇⢣⡿⠃                                       \n");
  
    print("  \n");

}


void game_over(Player *player) {
    clear_screen();

    if (player->energy <= 0) {
        pretty_print("You're too tired to continue... You fall asleep at your desk.\n");
    }

    if (player->charisma <= 0) {
        pretty_print("You're hated by all of your colleagues... You get fired!\n");
    }

    if (player->cash <= 0) {
        pretty_print("You're broke... .\n");
    }
    pretty_print("Better luck next time....!\n");
    print("\n");
    print("\n");
    print("\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⡀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⠀⠀⠀⢀⣴⣿⡶⠀⣾⣿⣿⡿⠟⠛⠁\n");
    print("⠀⠀⠀⠀⠀⠀⣀⣀⣄⣀⠀⠀⠀⠀⣶⣶⣦⠀⠀⠀⠀⣼⣿⣿⡇⠀⣠⣿⣿⣿⠇⣸⣿⣿⣧⣤⠀⠀⠀\n");
    print("⠀⠀⢀⣴⣾⣿⡿⠿⠿⠿⠇⠀⠀⣸⣿⣿⣿⡆⠀⠀⢰⣿⣿⣿⣷⣼⣿⣿⣿⡿⢀⣿⣿⡿⠟⠛⠁⠀⠀\n");
    print("⠀⣴⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⢠⣿⣿⣹⣿⣿⣿⣿⣿⣿⡏⢻⣿⣿⢿⣿⣿⠃⣼⣿⣯⣤⣴⣶⣿⡤⠀\n");
    print("⣼⣿⠏⠀⣀⣠⣤⣶⣾⣷⠄⣰⣿⣿⡿⠿⠻⣿⣯⣸⣿⡿⠀⠀⠀⠁⣾⣿⡏⢠⣿⣿⠿⠛⠋⠉⠀⠀⠀\n");
    print("⣿⣿⠲⢿⣿⣿⣿⣿⡿⠋⢰⣿⣿⠋⠀⠀⠀⢻⣿⣿⣿⠇⠀⠀⠀⠀⠙⠛⠀⠀⠉⠁⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠹⢿⣷⣶⣿⣿⠿⠋⠀⠀⠈⠙⠃⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠈⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣴⣶⣦⣤⡀⠀\n");
    print("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⣠⡇⢰⣶⣶⣾⡿⠷⣿⣿⣿⡟⠛⣉⣿⣿⣿⠆\n");
    print("⠀⠀⠀⠀⠀⠀⢀⣤⣶⣿⣿⡎⣿⣿⣦⠀⠀⠀⢀⣤⣾⠟⢀⣿⣿⡟⣁⠀⠀⣸⣿⣿⣤⣾⣿⡿⠛⠁⠀\n");
    print("⠀⠀⠀⠀⣠⣾⣿⡿⠛⠉⢿⣦⠘⣿⣿⡆⠀⢠⣾⣿⠋⠀⣼⣿⣿⣿⠿⠷⢠⣿⣿⣿⠿⢻⣿⣧⠀⠀⠀\n");
    print("⠀⠀⠀⣴⣿⣿⠋⠀⠀⠀⢸⣿⣇⢹⣿⣷⣰⣿⣿⠃⠀⢠⣿⣿⢃⣀⣤⣤⣾⣿⡟⠀⠀⠀⢻⣿⣆⠀⠀\n");
    print("⠀⠀⠀⣿⣿⡇⠀⠀⢀⣴⣿⣿⡟⠀⣿⣿⣿⣿⠃⠀⠀⣾⣿⣿⡿⠿⠛⢛⣿⡟⠀⠀⠀⠀⠀⠻⠿⠀⠀\n");
    print("⠀⠀⠀⠹⣿⣿⣶⣾⣿⣿⣿⠟⠁⠀⠸⢿⣿⠇⠀⠀⠀⠛⠛⠁⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    print("⠀⠀⠀⠀⠈⠙⠛⠛⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");

    exit();
}


int main() {
    Player player = create_player(); // Initialize player, create the object
    close_display();
    timer_init(); // Initialize timer
    
    /*
    char output[20]; // Adjusted memory reference
    read(output, 20);
    print(output);
    clear_screen();
    */

    morning_routine(&player); // Start game flow
    morning_routine(&player); // Start game flow

    morning_routine(&player); // Start game flow

    morning_routine(&player); // Start game flow

   


    /*

    while (1) {

    morning_routine(&player); // Move to the next stage
    choose_outfit(&player); // Move to the next stage
    breakfast(&player); // Move to the next stage

    car_game(&player); // Move to the next stage

    parkinglot(&player); // Move to the next stage
    desk(&player); // Move to the next stage

    meeting_game(&player); // Move to the next stage

    lunch(&player); // Move to the next stage
    toilet(&player); // Move to the next stage

    terminal_game(&player); // Move to the next stage

    clocking_out(&player); // Move to the next stage
       
    }
   


    */
    return 0;
}