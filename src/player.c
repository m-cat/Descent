#include "defs.h"
#include "actor.h"
#include "player.h"

ACTOR player;

/* Tries to perform a player action corresponding to the pressed key.
   Returns 1 if an action was peformed. */
#define move_if_can(p, d) (can_move(&p, d) ? (actor_move(&p, d), 1) : 0)
int handle_input(int key) {
  switch (key) {
  case '8':
  case KEY_UP:
    return move_if_can(player, DIR_N);
  case KEY_RIGHT:
  case '6':
    return move_if_can(player, DIR_E);
  case '2':
  case KEY_DOWN:
    return move_if_can(player, DIR_S);
  case '4':
  case KEY_LEFT:
    return move_if_can(player, DIR_W);
  case '9':
    return move_if_can(player, DIR_NE);
  case '3':
    return move_if_can(player, DIR_SE);
  case '1':
    return move_if_can(player, DIR_SW);
  case '7':
    return move_if_can(player, DIR_NW);
  default:
    return 0;
  }
}
