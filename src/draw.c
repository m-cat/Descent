#include "defs.h"
#include "util.h"
#include "draw.h"

void draw_warnings() {
  if (LINES < CON_HEIGHT || COLS < CON_WIDTH) {
    cprint(0, 0, C_RED_BLACK, "WARNING:");
    cprint(1, 0, C_RED_BLACK, "RESIZE  ");
    cprint(2, 0, C_RED_BLACK, "CONSOLE ");
    cprint(3, 0, C_RED_BLACK, "TO 80x24");
  }
}

void draw_all() {
  int i, j;
  for (i = 0; i < min(LINES, CON_HEIGHT); i++)
    for (j = 0; j < min(COLS, CON_WIDTH); j++) {
      if (i == PLAYER_Y && j == PLAYER_X)
	cprint(i, j, C_GREEN_BLACK, "@");
      else if (DUNGEON[i][j].type == TILE_FLOOR)
	cprint(i, j, C_WHITE_BLACK, " ");
      else if (DUNGEON[i][j].type == TILE_WALL)
	cprint(i, j, C_WHITE_BLACK, "#");
    }
  draw_warnings();
}
