#include <stdlib.h>
#include "defs.h"
#include "items.h"
#include "dungeon.h"

void dungeon_clear() {
  int i, j;
  DUNGEON_BLOCK *block;
  for (i = 0; i < CURRENT_HEIGHT; i++)
    for (j = 0; j < CURRENT_WIDTH; j++) {
      block = &DUNGEON[i][j];
      block->type = TILE_WALL;
      block->resident = NULL;
      block->furn = NULL;
      if (block->items != NULL)
	free(block->items);
      block->items = NULL;
    }
}

enum DIRECTION random_valid_path(int x, int y) {
  enum DIRECTION paths[4];
  int count = 0;
  if (y > 1 && DUNGEON[y-2][x].type == TILE_WALL)
    paths[count++] = DIR_N;
  if (x < CURRENT_WIDTH-4 && DUNGEON[y][x+2].type == TILE_WALL)
    paths[count++] = DIR_E;
  if (y < CURRENT_HEIGHT-4 && DUNGEON[y+2][x].type == TILE_WALL)
    paths[count++] = DIR_S;
  if (x > 1 && DUNGEON[y][x-2].type == TILE_WALL)
    paths[count++] = DIR_W;
  return count ? paths[rand_int(0,count-1)] : DIR_NONE;
}
void dungeon_gen_maze(int x, int y) {
  enum DIRECTION d;
  DUNGEON[y][x].type = TILE_FLOOR;
  while ((d = random_valid_path(x, y)) != DIR_NONE) {
    switch (d) {
    case DIR_N:
      DUNGEON[y-1][x].type = TILE_FLOOR;
      dungeon_gen_maze(x, y-2);
      break;
    case DIR_E:
      DUNGEON[y][x+1].type = TILE_FLOOR;
      dungeon_gen_maze(x+2, y);
      break;
    case DIR_S:
      DUNGEON[y+1][x].type = TILE_FLOOR;
      dungeon_gen_maze(x, y+2);
      break;
    case DIR_W:
      DUNGEON[y][x-1].type = TILE_FLOOR;
      dungeon_gen_maze(x-2, y);
      break;
    }
  }
}
void dungeon_gen_cave() {
  int i = 1, x=1, y=1;
  while ((float)i/(CURRENT_WIDTH*CURRENT_HEIGHT) < .5) {
    if (DUNGEON[y][x].type == TILE_WALL)
      i++;
    DUNGEON[y][x].type = TILE_FLOOR;
    switch (rand_int(DIR_N, DIR_W)) {
    case DIR_N:
      y -= (y>1);
      break;
    case DIR_E:
      x += (x < CURRENT_WIDTH-2);
      break;
    case DIR_S:
      y += (y < CURRENT_HEIGHT-2);
      break;
    case DIR_W:
      x -= (x>1);
      break;
    }
  }
  i = 10;
  while (i --> 0) {
    do {
      x = rand_int(0, CURRENT_WIDTH-1);
      y = rand_int(0, CURRENT_HEIGHT-1);
    } while (DUNGEON[y][x].type != TILE_FLOOR);
    item_place(y, x, "diamond");
    }
}

void dungeon_gen(enum DUNGEON_TYPE type) {
  dungeon_clear();
  switch (type) {
  case DUNGEON_MAZE:
    dungeon_gen_maze(1, 1);
    /* place_player() */
    break;
  case DUNGEON_CAVE:
    dungeon_gen_cave();
    break;
  }
}
