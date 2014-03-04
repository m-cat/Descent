#include <ncurses/ncurses.h>
#include "defs.h"
#include "util.h"
#include "init.h"
#include "dungeon.h"
#include "draw.h"

int main() {
  int i, j;

  /* Initialize game */
  init_all(); /* Initialize console and colors */
  dungeon_gen(DUNGEON_MAZE);

  /* MAIN GAME LOOP */
  while (!(getch()=='q')) {

    handle_resize();
    draw_all();
    msleep(1000/FPS); /* naive fps implementation */
  }

  /* Cleanup & exit */
  endwin();

  return 0;
}
