#include "defs.h"

/* Initialize variables */
uint DEPTH = 1;
uint MAX_WIDTH = 30, MAX_HEIGHT = 30;
int CURRENT_WIDTH = 0, CURRENT_HEIGHT = 0;
int DUNGEON_X = 0, DUNGEON_Y = 0;
int VIEW_WIDTH = CON_WIDTH - UI_WIDTH, VIEW_HEIGHT = CON_HEIGHT;
int UI_X = CON_WIDTH - UI_WIDTH, UI_Y = 0;

int CAMERA_X = 0, CAMERA_Y = 0;
int LOOK_X = 0, LOOK_Y = 0;

uint INPUT_MODE = INPUT_ACTION;
int INV_POS = 0;
unsigned long TURN_COUNT = 0;

/* Adjustable settings */
uint TURN_DELAY_ON = false;
uint SCROLL_FACTOR = 4;

/* Define global constants */
const char *GAME_NAME = "Descent";
