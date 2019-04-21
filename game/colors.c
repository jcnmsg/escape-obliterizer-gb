//SPRITES
#define RGB_LIGHTGREEN       RGB(16, 31, 27) 
#define RGB_DDARKGREEN       RGB(4, 18, 10)
#define RGB_LASERRED         RGB(31, 0, 0)
#define RGB_LASERVIOLET      RGB(25, 9, 29)
#define RGB_LASERDARKVIOLET  RGB(16, 7, 31)
#define RGB_ROSE             RGB(25, 0, 15)
#define RGB_DELLOWEENRED     RGB(31, 10, 8)
#define RGB_DELOWEENYELLOW   RGB(30, 29, 16)

//BG
#define RGB_VIOLET           RGB(7, 1, 6)
#define RGB_LIGHTVIOLET      RGB(14, 4, 12)
#define RGB_BGYELLOW         RGB(30, 30, 1)


const UWORD sprite_palettes[] = {
	0, RGB_LIGHTGREEN, RGB_DDARKGREEN, 0, // player sprite - regular skin
	0, RGB_LASERRED, RGB_LASERVIOLET, RGB_LASERDARKVIOLET, // laser sprite
    0, RGB_ROSE, RGB_ROSE, 0, // options colors
    0, RGB_DELOWEENYELLOW, RGB_DELLOWEENRED, 0,
};

const UWORD bkg_palettes[] = {
    RGB_DDARKGREEN, RGB_ROSE, RGB_LIGHTVIOLET, RGB_VIOLET // bg menu palette and game
};

const UWORD bkg_gameover_palette[] = {
    RGB_BGYELLOW, RGB_LIGHTVIOLET, RGB_LIGHTVIOLET, RGB_VIOLET // bg menu palette and game
};

const UWORD bkg_credits_palette[] = {
    RGB_ROSE, RGB_LIGHTVIOLET, RGB_LIGHTVIOLET, RGB_VIOLET // bg menu palette and game
};

void initColors() { 
    set_sprite_palette(0, 4, sprite_palettes); // Set sprite palettes to memory
}