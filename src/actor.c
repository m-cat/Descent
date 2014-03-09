#include "defs.h"
#include "actor.h"

int can_move(ACTOR *a, enum DIRECTION d) {
  switch (d) {
  case DIR_N:
    return (DUNGEON[a->y-1][a->x].type == TILE_FLOOR);
  case DIR_E:
    return (DUNGEON[a->y][a->x+1].type == TILE_FLOOR);
  case DIR_S:
    return (DUNGEON[a->y+1][a->x].type == TILE_FLOOR);
  case DIR_W:
    return (DUNGEON[a->y][a->x-1].type == TILE_FLOOR);
  case DIR_NE:
    return (DUNGEON[a->y-1][a->x+1].type == TILE_FLOOR);
  case DIR_SE:
    return (DUNGEON[a->y+1][a->x+1].type == TILE_FLOOR);
  case DIR_SW:
    return (DUNGEON[a->y+1][a->x-1].type == TILE_FLOOR);
  case DIR_NW:
    return (DUNGEON[a->y-1][a->x-1].type == TILE_FLOOR);
  }
}
void actor_move(ACTOR *a, enum DIRECTION d) {
  DUNGEON[a->y][a->x].resident = NULL;
  switch (d) {
  case DIR_N:
    a->y -= 1; break;
  case DIR_E:
    a->x += 1; break;
  case DIR_S:
    a->y += 1; break;
  case DIR_W:
    a->x -= 1; break;
  case DIR_NE:
    a->x += 1; a->y -= 1; break;
  case DIR_SE:
    a->x += 1; a->y += 1; break;
  case DIR_SW:
    a->x -= 1; a->y += 1; break;
  case DIR_NW:
    a->x -= 1; a->y -= 1; break;
  }
  DUNGEON[a->y][a->x].resident = a;
}
