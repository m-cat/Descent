#include <stdlib.h>
#include <assert.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "dungeon.h"
#include "actor.h"
#include "player.h"
#include "algorithms.h"
#include "io.h"
#include "menu.h"
#include "init.h"

int main() {
  int next_turn;
  TCOD_key_t key;
  TCOD_event_t ev;
  TCOD_mouse_t mouse;

  /* Initialize game */
  init_all(); /* Initialize console and colors */

  /* Main menu */
  handle_menu();

  /* Generate dungeon */
  dungeon_gen(DUNGEON_CAVE);

  calc_fov();
  draw_game();

  /* MAIN GAME LOOP */
  do {
    ev = TCOD_sys_wait_for_event(TCOD_EVENT_KEY|TCOD_EVENT_MOUSE_PRESS,
				 &key,&mouse,1);
    next_turn = handle_input(ev, key.vk, key.c, key.lctrl||key.rctrl,
			     mouse.cx, mouse.cy);
    if (next_turn) { /* if player used up a turn */
      TURN_COUNT++;
      advance_turn();
      calc_fov();
    }
    draw_game();
  } while (!TCOD_console_is_window_closed() && key.c != 'q');

  exit(1);
}
