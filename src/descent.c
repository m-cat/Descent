#include <ncurses/ncurses.h>
#include "defs.h"
#include "util.h"
#include "init.h"
#include "dungeon.h"
#include "draw.h"

int main() {
  int i, j;

  init_all(); /* Initialize console and colors */
  dungeon_gen(DUNGEON_MAZE);

  /* MAIN GAME LOOP */
  while (!(getch()=='q')) {
    /*for (i = 0; i < LINES; i++)
      for (j = 0; j < COLS; j++) {
	if (j == COLS-1)
	  cprint(i, j, C_RED_BLACK, "x");
	else
	  cprint(i, j, C_BLUE_BLACK, "o");
      }
    */
    handle_resize();
    draw_all();
    msleep(1000/FPS); /* naive fps implementation */
  }
  endwin();

  return 0;
}
