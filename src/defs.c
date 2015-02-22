#include "defs.h"

/* Initialize variables */
int			DEPTH = 1;
int			MAX_WIDTH = 128, MAX_HEIGHT = 128;
int			CURRENT_WIDTH = 80, CURRENT_HEIGHT = 50;
int			DUNGEON_X = 0, DUNGEON_Y = 0;
int			VIEW_WIDTH = CON_WIDTH - UI_WIDTH, VIEW_HEIGHT = CON_HEIGHT;
int			UI_X = CON_WIDTH - UI_WIDTH, UI_Y = 0;	/* UI_X = VIEW_WIDTH */

int			INPUT_MODE = INPUT_ACTION;
int			CAMERA_X = 0, CAMERA_Y = 0;
int			LOOK_X = 0, LOOK_Y = 0;
int			TURN_COUNT = 0;
int			INV_POS = 0;

/* Adjustable settings */
int			TURN_DELAY_ON = false;

/* Define global constants */
const char	*GAME_NAME = "Descent";
