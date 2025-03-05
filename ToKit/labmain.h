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
    int secret;
    int plotting;

    // Items
    int item1;
    int item2;
    int item3;
    int item4;
    int hat;

} Player;

extern void print(const char *str);
extern void print_dec(unsigned int x);
extern void enable_interrupts( void );
extern void disable_interrupts( void );
void handle_interrupt();
//extern void read(char *, int);
void clear_screen();
void clear_row();
void delay(unsigned int milli);
void pretty_print(const char text[]);
void set_leds(int led_mask);
void print_status(Player *player);

//////////////////GAME FUNCTIONS////////////////////
void morning_routine(Player *player);
void choose_outfit(Player *player);
void breakfast(Player *player);

//void car_game(Player *player); //FPGA GAME /Lose Money

void parkinglot(Player *player);
void desk(Player *player);

void coffee_game(Player *player); //FPGA GAME / Lose Energy
void set_coffee();
void print_sak(const char* bild[]);
void display_energy();

void lunch(Player *player);
void toilet(Player *player);

// CAR GAME / Lose money
#define NUM_LANES 3
#define ROAD_HEIGHT 20

char enemy_lanes[] = "0000000000000v0000000000000000v0v00000000000000000000#00#00#0000000000#00#00#0000000000#00#00#00000000000000000#0##0##0#0000000000000v000v0v000v0v000v0v0000000000v00v00000000vv0000vv0000vv00000000000000vv0000vv0000vv000000000#00##0##00#000000000000000v0v000v0v0000000000v000v0v000v000000000#00#00#000000000000000000000000000000000000000000000000000000000000000";

void car_game(Player *player);
int get_switches_event();
void display_krockar();
void draw_road();

//void terminal_game(Player *player);

void clocking_out(Player *player);
void stat_check(Player *player);
void game_over(Player *player);