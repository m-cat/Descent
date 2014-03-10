#include "defs.h"
#include "actor.h"
#include "player.h"

ACTOR player;

/* Tries to perform a player action corresponding to the pressed key.
   Returns 1 if an action was peformed. */
#define move_if_can(p, d) (can_move(&p, d) ? (actor_move(&p, d), 1) : 0)
int handle_input(TCOD_keycode_t key) {
  switch (key) {
  case TCODK_KP8:
  case TCODK_UP:
    return move_if_can(player, DIR_N);
  case TCODK_RIGHT:
  case TCODK_KP6:
    return move_if_can(player, DIR_E);
  case TCODK_KP2:
  case TCODK_DOWN:
    return move_if_can(player, DIR_S);
  case TCODK_KP4:
  case TCODK_LEFT:
    return move_if_can(player, DIR_W);
  case TCODK_KP9:
    return move_if_can(player, DIR_NE);
  case TCODK_KP3:
    return move_if_can(player, DIR_SE);
  case TCODK_KP1:
    return move_if_can(player, DIR_SW);
  case TCODK_KP7:
    return move_if_can(player, DIR_NW);
  case TCODK_KP5:
    return 1; /* take up turn doing nothing */
  case TCODK_PRINTSCREEN:
  case TCODK_F12:
    TCOD_sys_save_screenshot(NULL);
    return 0;
  default:
    return 0;
  }
}
