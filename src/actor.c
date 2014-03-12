#include "defs.h"
#include "actor.h"

int can_move(ACTOR *a, int dy, int dx) {
  return CHK_PASSABLE(a->y+dy, a->x+dx);
}
void actor_move(ACTOR *a, int dy, int dx) {
  DUNGEON[a->y][a->x].resident = NULL;
  a->y += dy;
  a->x += dx;
  DUNGEON[a->y][a->x].resident = a;
}
