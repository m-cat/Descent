#include "defs.h"
#include "util.h"
#include "player.h"
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
  int i, j, drawi, drawj;
  int scr_width = min(COLS, CON_WIDTH)-UI_WIDTH;
  int scr_height = min(LINES, CON_HEIGHT);
  dungeon_block *block;
  actor *a;
  for (i = player.y-scr_height/2+1; i < player.y+scr_height/2; i++)
    for (j = player.x-scr_width/2+UI_WIDTH/2+1; j < player.x+scr_width/2; j++) {
      drawi = i-player.y+scr_height/2-1, drawj = j-player.x+scr_width/2-1;
      /* Check if out of bounds */
      if (i < 0 || i >= CURRENT_HEIGHT || j < 0 || j >= CURRENT_WIDTH) {
	cprint(drawi, drawj, C_WHITE_BLACK, " ");
	continue;
      }
      block = &DUNGEON[i][j];
      a = block->resident;
      if (a != NULL)
	cprint(drawi, drawj, get_actor_color(a), "%c", a->ch);
      else if (block->type == TILE_FLOOR)
	cprint(drawi, drawj, C_WHITE_BLACK, " ");
      else if (block->type == TILE_WALL)
	cbprint(drawi, drawj, C_WHITE_BLACK, "#");
    }
  draw_warnings();
}
