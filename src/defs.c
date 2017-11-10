#include "defs.h"

/* Initialize variables */
uint DEPTH = 1;
uint MAX_WIDTH = 128, MAX_HEIGHT = 128;
uint CURRENT_WIDTH = 80, CURRENT_HEIGHT = 50;
uint DUNGEON_X = 0, DUNGEON_Y = 0;
uint VIEW_WIDTH = CON_WIDTH - UI_WIDTH, VIEW_HEIGHT = CON_HEIGHT;
uint UI_X = CON_WIDTH - UI_WIDTH, UI_Y = 0;

uint INPUT_MODE = INPUT_ACTION;
uint CAMERA_X = 0, CAMERA_Y = 0;
uint LOOK_X = 0, LOOK_Y = 0;
uint INV_POS = 0;
unsigned long TURN_COUNT = 0;

/* Adjustable settings */
uint TURN_DELAY_ON = false;
uint SCROLL_FACTOR = 4;

/* Define global constants */
const char *GAME_NAME = "Descent";
