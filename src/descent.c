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
#include "priority.h"

int main() {
  int next_turn;
  TCOD_key_t key;
  TCOD_event_t ev;
  TCOD_mouse_t mouse;

  /* Initialize game */
  init_all(); /* Initialize console and colors */

 start:

  /* Main menu */
  if (handle_menu() == 1) {
	  exit(1);
  }

  /* Generate dungeon */
  dungeon_gen(DUNGEON_CAVE);

  calc_fov();
  draw_game();

  /* MAIN GAME LOOP */
  do {
    ev = TCOD_sys_wait_for_event(TCOD_EVENT_KEY|TCOD_EVENT_MOUSE_PRESS,&key,&mouse,0);
	if (TCOD_console_is_window_closed()) {
		game_end();
	}

    next_turn = handle_input(ev, key.vk, key.c, key.lctrl||key.rctrl, mouse.cx, mouse.cy);
    if (next_turn == 1) { /* if player used up a turn */
      TURN_COUNT++;
      advance_turn();
      calc_fov();
    }
    draw_game();
  } while (next_turn >= 0);

  if (next_turn == -1) /* Escape was pressed, return to menu */
    goto start;

  exit(1);
}

/* Advances a turn for ALL actors apart from player. */
void advance_turn() {
  ACTOR *a;
  int pri;
  while ((a = priq_pop(actor_queue, &pri))) {
    /* Take out actor from the queue, act, put it in a temp queue */
    actor_act(a);
	if (TURN_DELAY_ON && CHK_VISIBLE(a->y,a->x)) {
		TCOD_sys_sleep_milli(TURN_DELAY);
	    draw_game();
	}
    priq_push(temp_queue, a, pri);
  }
  /* Put all actors in the temp queue back into the main queue */
  priq_combine(actor_queue, temp_queue);
}
