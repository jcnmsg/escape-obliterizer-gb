// GAME BACKGROUNDS
#include "../background/game/backgroundmap.c"
#include "../background/game/backgroundtiles.c"

// GAME SPRITES
#include "../sprites/player.c"
#include "../sprites/verticallaser.c"
#include "../sprites/horizontallaser.c"
#include "../sprites/lasergun.c"
#include "../sprites/bomblaser.c"
#include "../sprites/stun.c"
#include "../sprites/stunqueue.c"
#include "../sprites/explosion.c"

struct Laser {
    UBYTE repetitions[10];
}

UINT8 h_laser;
UINT8 hlaser_ready = 1;
UINT8 hlaser_pos, h_current_clock;
 
struct Laser v_laser;
UINT8 vlaser_ready = 1; 
UINT8 v_laser_pos, v_current_clock;

UINT8 b_ready = 1;
UINT8 b_pos, b_current_clock;

UINT8 player_x, player_y;
font_t ibm;// Declare font variable

void count_score() {
    gotoxy(1, 16); // Position of the console on screen, uses tiles so x=1*8 and y=16*8
    printf("%u", score++); // Print score at desired position
}

void reset_player_position(){
    UINT8 w;
    for (w = 0; w <= 5; w++) {
        if (w == selected_skin) {
            set_sprite_tile(39, 11+w+w);
            if (selected_skin < 2) {
                set_sprite_prop(39, 0);
            }
            if (selected_skin >= 2){
                set_sprite_prop(39, selected_skin + 1);
            }
        }
    }
        
    set_sprite_tile(37, 48);
    set_sprite_prop(37, 7);
    set_sprite_tile(38, 48);
    set_sprite_prop(38, get_sprite_prop(37) | S_FLIPX);
    
    move_sprite(37, 200, 200); //hide stun queue
    move_sprite(38, 200, 200); //hide reversed stun queue

    move_sprite(39, 84, 80); // reset player

    player_x = 84;
    player_y = 80;
    if (stunned == 0)  {
        stun = 0;
    }
}

void init_game_loop(){
    font_init(); // Initialize font library after state change to avoid overwriting bg tiles
    color(DKGREY, BLACK, SOLID); // Customize colors of font 
    ibm = font_load(font_ibm); // Load built in font_ibm
    font_set(ibm); // Set built in font_ibm, will be used on displaying the score, only 36 tiles
    set_bkg_palette(0, 1, bkg_game_palette);
    set_bkg_data(109, 6, BackgroundTiles); // Sets which background tileset to use, starts on 39, after the font load, counts three tiles
    set_bkg_tiles(0, 0, 20, 14, BackgroundMap); // Sets which background map to use and position on screen starting on x=0, y=0 (offscreen) and spanning 20x18 tiles of 8 pixels each
    set_sprite_data(0, 30, Player); // Sets the player sprite, starts on zero, counts seven
    set_sprite_data(30, 18, VerticalLaser); // Sets the vertical laser sprites
    set_sprite_data(94, 4, HorizontalLaser); // Sets the horizontal laser sprites
    set_sprite_data(100, 12, LaserGun); // Sets the laser gun sprites
    set_sprite_data(48, 8, StunQueue); // Sets the stun queue sprites
    set_sprite_data(58, 10, TheStun); // Sets the stun sprites
    set_sprite_prop(36, 6); // Setup stun animation colors
    set_sprite_data(68, 6, BombLaser); // Sets the bomb laser sprites
    set_sprite_data(76, 14, Explosion); // Sets the explosion sprites
    set_sprite_prop(17, 6);
    set_sprite_prop(18, get_sprite_prop(17) | S_FLIPX);
    SHOW_SPRITES; // Draw sprites
    reset_player_position();
    DISPLAY_ON;
    stun = 0;
    stunned = 0;
    pattern_duty = 2;
}
 
void draw_the_vlaser(struct Laser* laser, UINT8 x, UINT8 y) {
    UINT8 i; 
    for (i = 1; i < 10; i++ ){ 
        move_sprite(laser->repetitions[i], x, y + (16 * i));
    } 
}

void draw_the_hlaser(UINT8 x, UINT8 y) {
    x = 0;
    if (y == 36 || y == 116) {
        move_sprite(19, 80, y);
        move_sprite(20, 88, y);
    }
    else if (y == 76) {
        move_sprite(19, 40, y);
        move_sprite(20, 48, y);
        move_sprite(21, 84, y);
        move_sprite(22, 120, y);
        move_sprite(23, 128, y);
    }
}

void draw_the_bomb(UINT8 x, UINT8 y) {
    UINT8 i; 
    for(i = 0; i < 8; i++){
        if (i == 0) {
            move_sprite(9+i, x - 16, y - 20);
        }
        if (i == 1) {
            move_sprite(9+i, x - 8 , y - 20);
        }
        if (i == 2) {
            move_sprite(9+i, x + 16, y - 20);
        }
        if (i == 3) {
            move_sprite(9+i, x + 8 , y - 20);
        }
        if (i == 4) {
            move_sprite(9+i, x - 16, y + 12);
        }
        if (i == 5) {
            move_sprite(9+i, x - 8 , y + 12);
        }
        if (i == 6) {
            move_sprite(9+i, x + 16, y + 12);
        }
        if (i == 7) {
            move_sprite(9+i, x + 8 , y + 12);
        }
    }
}

void trigger_vlaser(UINT8 x) { // Trigger Vertical Laser, only requires x coordinate, y = 0
    UINT8 i;
    v_laser_pos = x;
    v_current_clock = (clock() / CLOCKS_PER_SEC);
    for(i = 0; i < 9; i++){
        v_laser.repetitions[i] = i;
        set_sprite_tile(i, 30);
        set_sprite_prop(i, 1);
    }
    draw_the_vlaser(&v_laser, x, 0);
}

void trigger_hlaser(UINT8 y) { // Trigger Horiziontal Laser, only requires y coordinate, x = 0
    UINT8 i;
    hlaser_pos = y;
    h_current_clock = (clock() / CLOCKS_PER_SEC);
    if (y == 36 || y == 116) {
        set_sprite_tile(19, 94);
        set_sprite_prop(19, 1);
        set_sprite_tile(20, 94);
        set_sprite_prop(20, 1);
        if (y == 36) {
           set_bkg_tiles(0,3,20,1, LaserBgMapSingleBase);
        }
        if (y == 116) {
           set_bkg_tiles(0,13,20,1, LaserBgMapSingleBase);
        }
    }
    else if (y == 76) {
        for(i = 0; i < 6; i++){
            set_sprite_tile(19 + i, 94);
            set_sprite_prop(19 + i, 1);
            set_bkg_tiles(0,8,20,1, LaserBgMapDualBase);
        }
    }
    
    draw_the_hlaser(0, y);
}


void trigger_bomb(UINT8 x) {
    UINT8 i;
    b_pos = x;
    b_current_clock = (clock() / CLOCKS_PER_SEC);
    for(i = 0; i < 8; i++){
        if (i < 2) {
            set_sprite_tile(9+i, 68+i+i);
        }
        if (i >= 2 && i < 4) {
            set_sprite_tile(9+i, 68+i+i-2-2);
        }
        if (i >= 4 && i < 6) {
            set_sprite_tile(9+i, 68+i+i-4-4);
        }
        if (i >= 6 && i < 8) {
            set_sprite_tile(9+i, 68+i+i-6-6);
        }
        set_sprite_prop(9+i, 1);
        if (i >= 2 && i < 4) {
            set_sprite_prop(9+i, get_sprite_prop(9+i) | S_FLIPX);
        }
        if (i >=4 && i < 6) {
            set_sprite_prop(9+i, get_sprite_prop(9+i) | S_FLIPY);
        }
        if (i>=6){
            set_sprite_prop(9+i, get_sprite_prop(9+i) | S_FLIPX | S_FLIPY);
        }
    }
    draw_the_bomb(x, 80);
}

void is_vlaser_ready_to_blow() {
    UINT8 i, z;
    if ( (clock() / CLOCKS_PER_SEC) - v_current_clock >= 1 && vlaser_ready == 0 && state !=3 ) {
        play_sound_fx(0);
        for (z = 2; z < 20; z+=2) {
            for(i = 0; i < 9; i++){
                v_laser.repetitions[i] = i;
                set_sprite_tile(i, 28+z);
            }
            draw_the_vlaser(&v_laser, v_laser_pos, 0);
            set_delay(2);
        }
        if (player_x == v_laser_pos) {
            for (i = 0; i <= 5; i++) {
                set_sprite_tile(39, 23+i);
                set_delay(3);
            }
            DISPLAY_OFF;
            player_x = 0;
            init_game_over();
            state = 3;
        }
        vlaser_ready = 1;
    }
    else if (state == 3) {
        vlaser_ready = 1;
    }
}

void is_hlaser_ready_to_blow() {
    UINT8 i;
    if ( (clock() / CLOCKS_PER_SEC) - h_current_clock >= 1 && hlaser_ready == 0 && state !=3 ) {
        play_sound_fx(3);
        set_bkg_tiles(0, 0, 20, 14, BackgroundMap); // Reset full bgmap
        for (i = 0; i < 5; i++) {
            move_sprite(19+i, 200, 200);
        }
        // animate hazard
        for (i = 0; i < 14; i++) {
            set_delay(2);
        }
        if (player_y == hlaser_pos + 4) {
            for (i = 0; i <= 5; i++) {
                set_sprite_tile(39, 23+i);
                set_delay(3);
            }
            DISPLAY_OFF;
            player_y = 0;
            init_game_over();
            state = 3;
        }
        hlaser_ready = 1;
    }
    else if (state == 3) {
        hlaser_ready = 1;
    }
}

void is_bomb_ready_to_blow() {
    UINT8 i;
    if ( (clock() / CLOCKS_PER_SEC) - b_current_clock >= 1 && b_ready == 0 && state != 3) {
        play_sound_fx(2);
        set_sprite_tile(17, 76);
        set_sprite_tile(18, 76);
        move_sprite(17, b_pos-4, 76); // reset player
        move_sprite(18, b_pos+4, 76); // reset player
        for(i = 9; i < 17; i++ ) {
            move_sprite(i, 200, 200);
        }
        for(i = 0; i < 18; i+=2){
            set_sprite_tile(17, 76+i);
            set_sprite_tile(18, 76+i);
            set_delay(2);
        }
        if (player_x == b_pos) {
            for (i = 0; i <= 5; i++) {
                set_sprite_tile(39, 23+i);
                set_delay(3);
            }
            DISPLAY_OFF;
            player_x = 0;
            init_game_over();
            state = 3;
        }
        b_ready = 1;
    }
    else if (state == 3) {
        b_ready = 1;
    }
}

void call_vlaser(){
    unsigned UINT8 pos = generate_random_num(3);
    if (pos == 0) {
        vlaser_ready = 0;
        trigger_vlaser(44);
    }
    else if (pos == 1) {
        vlaser_ready = 0;
        trigger_vlaser(84);
    }
    else if (pos == 2) {
        vlaser_ready = 0;
        trigger_vlaser(124);
    }
    else if (pos == 3) {
        vlaser_ready = 1;
    }
}

void call_bomb() {
    unsigned UINT8 pos = generate_random_num(2);
    if (pos == 0) {
        b_ready = 0;
        trigger_bomb(44);
    }
    else if (pos == 1) {
        b_ready = 0;
        trigger_bomb(124);
    }
    else if (pos == 2) {
        b_ready = 1;
    }
}

void call_hlaser() {
    unsigned UINT8 pos = generate_random_num(3);
    if (pos == 0) {
        hlaser_ready = 0;
        trigger_hlaser(36);
    }
    else if (pos == 1) {
        hlaser_ready = 0;
        trigger_hlaser(76);
    }
    else if (pos == 2) {
        hlaser_ready = 0;
        trigger_hlaser(116);
    }
    else if (pos == 3) {
        hlaser_ready = 1;
    }
}

void start_hazards() {
    unsigned UINT8 hazard;
    if ( ((clock() / CLOCKS_PER_SEC)) % 2 == 0) { // change for timer 
        hazard = generate_random_num(2);
        if (hazard == 0 && vlaser_ready == 1) {
            call_vlaser();
        }
        else if (hazard == 1 && hlaser_ready == 1) {
            call_hlaser();
        }
        else if (hazard == 2 && b_ready == 1) {
            call_bomb();
        }
    }
}

void play_stun_animation() {
    move_sprite(36, 85, 78);
    if ((stun > 0 && stun <= 5) || (stun > 25 && stun <= 30) || (stun > 50 && stun <= 55) ) {
        set_sprite_tile(36, 58);
    }
    if ((stun > 5 && stun <= 10) || (stun > 30 && stun <= 35) || (stun > 55 && stun <= 60)) {
        set_sprite_tile(36, 60);
    }
    if ((stun > 10 && stun <= 15) || (stun > 35 && stun <= 40) || (stun > 60 && stun <= 65)) {
        set_sprite_tile(36, 62);
    }
    if ((stun > 15 && stun <= 20) || (stun > 40 && stun <= 45) || (stun > 65 && stun <= 70)) {
        set_sprite_tile(36, 64);
    }
    if ((stun >= 20 && stun <= 25) || (stun > 45 && stun <= 50) || (stun > 70 && stun <= 75)) {
        set_sprite_tile(36, 66);
    }
    if (stun == 1 || stun == 12 || stun == 40 || stun == 52) {
        play_sound_fx(1);
    }
}

void process_stun() {
    if (stunned == 1) {
        play_stun_animation();
        if(stun <= 75){ 
            stun++;
        }
        else {
            stunned = 0;
            stun = 0;
            move_sprite(36, 200, 200); // hide stun sprite on stun finish
        }
    } 
}

void animate_stun_queue(){
    if (stun > 0 && stun < 20) {
        set_sprite_tile(37, 50);
        set_sprite_tile(38, 50);
    }
    if (stun >=20 && stun < 40) {
        set_sprite_tile(37, 52);
        set_sprite_tile(38, 52);
    } 
    if (stun >=40 && stun < 60) {
        set_sprite_tile(37, 54);
        set_sprite_tile(38, 54);
    } 
    if (stun >= 60 && stun < 80) {
        set_sprite_tile(37, 119);
        set_sprite_tile(38, 119);
    }
    score+=3;
}