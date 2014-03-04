#include "defs.h"
#include "util.h"
#include "draw.h"

void draw_all() {
  int i, j;
  for (i = 0; i < min(LINES, CURRENT_HEIGHT); i++)
    for (j = 0; j < min(COLS, CURRENT_WIDTH); j++) {
      if (i == PLAYER_Y && j == PLAYER_X)
	cprint(i, j, C_GREEN_BLACK, "@");
      else if (DUNGEON[i][j].type == TILE_FLOOR)
	cprint(i, j, C_WHITE_BLACK, " ");
      else if (DUNGEON[i][j].type == TILE_WALL)
	cprint(i, j, C_WHITE_BLACK, "#");
    }
}
