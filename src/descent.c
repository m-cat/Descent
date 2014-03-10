#include <stdlib.h>
#include <assert.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "dungeon.h"
#include "player.h"
#include "algorithms.h"
#include "io.h"
#include "init.h"

int main() {
  int i, j, next_turn;
  TCOD_key_t key;

  /* Initialize game */
  init_all(); /* Initialize console and colors */

  /* Main menu */
  handle_menu();
  assert (strlen(player.name) > 0);

  /* Generate dungeon */
  dungeon_gen(DUNGEON_CAVE);
  /* Initialize player */
  player.x = 1; /* temporary */
  player.y = 1;
  player.ch = '@';
  DUNGEON[1][1].resident = &player;

  calc_fov();
  draw_game();

  /* MAIN GAME LOOP */
  do {
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS,&key,NULL,1);
    if (!TCOD_console_is_active())
      continue;
    next_turn = handle_input(key.vk);
    if (next_turn) { /* if player used up a turn */
      //advance_turn();
      calc_fov();
    }
    draw_game();
  } while (key.c != 'q');

  exit(1);
}
