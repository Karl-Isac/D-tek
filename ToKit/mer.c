#define LANE_WIDTH 5
#define NUM_LANES 3
#define ROAD_WIDTH (LANE_WIDTH * NUM_LANES)
#define ROAD_HEIGHT 10

int player_lane = 1; // Start in the middle lane
char enemy_lanes[] = "0000000000000v0000000000000000v0v00000000000000000000#00#00#0000000000#00#00#0000000000#00#00#00000000000000000#0##0##0#0000000000000v000v0v000v0v000v0v0000000000v00v00000000vv0000vv0000vv00000000000000vv0000vv0000vv000000000#00##0##00#000000000000000v0v000v0v0000000000v000v0v000v000000000#00#00#000";
int progress = 0;
int krockar = 0;

void draw_road() {
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
            if (enemy_lanes[i] != '0') krockar++;
        } else {
            char out = (enemy_lanes[3] == '0') ? ' ' : enemy_lanes[i];
            print(out);
        }
        print(" | ");
    }
    progress++;
}

int get_switches_event() {
    volatile int* switches = (volatile int*) 0x04000010;
    switches += 3;
    int ret =  *switches;
    *switches = 0;
    return ret;
}

int main() {
    int dir;
    while (progress < 100) {
        delay(250);
        if ((dir = get_switches_event()) != 0) {
            if (dir % 2 == 0 && player_lane > 0) player_lane--;
             else if (player_lane < NUM_LANES - 1) player_lane++;
        }
        draw_road();
    }
    return 0;
}