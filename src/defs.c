#include "defs.h"

/* Initialize variables */
unsigned int    DEPTH = 1;
unsigned int    MAX_WIDTH = 128, MAX_HEIGHT = 128;
unsigned int    CURRENT_WIDTH = 80, CURRENT_HEIGHT = 50;
unsigned int    DUNGEON_X = 0, DUNGEON_Y = 0;
unsigned int    VIEW_WIDTH = CON_WIDTH - UI_WIDTH, VIEW_HEIGHT = CON_HEIGHT;
unsigned int    UI_X = CON_WIDTH - UI_WIDTH, UI_Y = 0;

unsigned int    INPUT_MODE = INPUT_ACTION;
unsigned int    CAMERA_X = 0, CAMERA_Y = 0;
unsigned int    LOOK_X = 0, LOOK_Y = 0;
unsigned int    TURN_COUNT = 0;
unsigned int    INV_POS = 0;

/* Adjustable settings */
unsigned int    TURN_DELAY_ON = false;
unsigned int    SCROLL_FACTOR = 4;

/* Define global constants */
const char      *GAME_NAME = "Descent";
