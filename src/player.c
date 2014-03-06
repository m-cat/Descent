#include "defs.h"
#include "actor.h"
#include "player.h"

actor player;

/* Tries to perform a player action corresponding to the pressed key.
   Returns 1 if an action was peformed. */
#define move_if_can(p, d) (can_move(&p, d) ? (actor_move(&p, d), 1) : 0)
int handle_input(int key) {
  switch (key) {
  case KEY_UP:
    return move_if_can(player, DIR_N);
  case KEY_RIGHT:
    return move_if_can(player, DIR_E);
  case KEY_DOWN:
    return move_if_can(player, DIR_S);
  case KEY_LEFT:
    return move_if_can(player, DIR_W);
  default:
    return 0;
  }
}
