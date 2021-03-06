

// GLOBAL VARIABLES NEEDED ACROSS STATES
extern int highscore;
int score = 0; 
int stun = 0;
int stunned = 0;
UINT8 state = 1;

// GAMEPLAY
UINT8 generate_random_num(UINT8 upper) { // Generates random with upper as maximum
    UINT8 num;
    num = (rand() % (upper + 1) );
    return num;
}

void set_delay(UINT8 loops) {
    UINT8 i;
    for (i = 0; i < loops; i++){
        wait_vbl_done();
    }
}

// SOUND
void play_sound_fx(UINT8 fx) {
    
    NR52_REG = 0x80; 
    NR51_REG = 0xFF; 
    NR50_REG = 0x77;

    if (fx == 0) { // Laser blowing sound        
        NR10_REG = 4 | (1 << 3) | (2 << 4); 
        NR11_REG = 1 | (3 << 6); 
        NR12_REG = 1 | (0 << 3) | (15 << 4); 
        NR13_REG = 0xFF & 1020; 
        NR14_REG = 0xC3; 
    }

    if (fx == 1) { // Stun sound fx (Frequency: 1936)
        NR21_REG = 0xC0;         
        NR22_REG = 0x74; 
        NR23_REG = 0x90;
        NR24_REG = 0xC7;
    }

    if (fx == 2) { // bomb sound fx
        NR41_REG = 1;
        NR42_REG = 1 | (0 << 3) | (15 << 4);
        NR43_REG =  1 | (2 << 3) | (2 << 4);
        NR44_REG = (1 << 6) | (1 << 7);
    }

    if (fx == 3) { // laser gun sound fx
        NR10_REG = 5 | (1 << 3) | (2 << 4);
        NR11_REG = 1 | (3 << 6);
        NR12_REG = 3 | (0 << 3) | (15 << 4);
        NR13_REG = 0xFF & 1651;
        NR14_REG = 1651 >> 8 | (0 << 6) | (1 << 7);
    }
}

void clear_sprite_positions() {
    UINT8 i;
    for (i = 0; i < 30; i++ ){
        move_sprite(i, 200, 200);
    }
}