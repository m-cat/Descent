#include <ncurses/ncurses.h>
#include "defs.h"
#include "util.h"
#include "dungeon.h"
#include "player.h"
#include "io.h"
#include "init.h"

int main() {
  int i, j, key, next_turn;

  /* Initialize game */
  init_all(); /* Initialize console and colors */

  /* Main menu */
  handle_menu();

  /* Generate dungeon */
  dungeon_gen(DUNGEON_MAZE);
  /* Initialize player */
  player.x = 1; /* temporary */
  player.y = 1;
  player.ch = '@';
  DUNGEON[1][1].resident = &player;

  /* MAIN GAME LOOP */
  while ((key=getch()) != 'q') {
    next_turn = handle_input(key);
    if (next_turn) /* if player used up a turn */
      ;//advance_turn();
    handle_resize();
    draw_game();
    msleep(1000/FPS); /* naive fps implementation */
  }

  /* Cleanup & exit */
  endwin();

  return 0;
}
