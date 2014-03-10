#include <stdlib.h>
#include "defs.h"
#include "items.h"
#include "dungeon.h"

void SET_WALL(y, x) {
  DUNGEON[y][x].type = TILE_WALL;
  SET_TRANSPARENT(y, x, 0);
  SET_PASSABLE(y, x, 0);
}
void SET_FLOOR(y, x) {
  DUNGEON[y][x].type = TILE_FLOOR;
  SET_TRANSPARENT(y, x, 1);
  SET_PASSABLE(y, x, 1);
}
void CLR_OPTS(y, x) {
  DUNGEON[y][x].PASSABLE = 0;
  DUNGEON[y][x].TRANSPARENT = 0;
  DUNGEON[y][x].VISIBLE = 0;
  DUNGEON[y][x].EXPLORED = 0;
}

void dungeon_clear() {
  int i, j;
  DUNGEON_BLOCK *block;
  ITEM_STACK *items;
  ITEM_STACK *temp;
  for (i = 0; i < CURRENT_HEIGHT; i++) {
    for (j = 0; j < CURRENT_WIDTH; j++) {
      SET_WALL(i, j);
      CLR_OPTS(i, j);
      block = &DUNGEON[i][j];
      block->resident = NULL;
      block->furn = NULL;
      items = block->items;
      /* Delete every node in a stash */
      while (items != NULL) {
	temp = items->next;
	free(items);
	items = temp;
      }
      block->items = NULL;
    }
  }
  TCOD_map_clear(fov_map, 0, 0);
}

void dungeon_set_fov() {
  int i, j;
  for (i = 0; i < CURRENT_HEIGHT; i++)
    for (j = 0; j < CURRENT_WIDTH; j++)
      TCOD_map_set_properties(fov_map, j, i,
			      CHK_TRANSPARENT(i, j),
			      CHK_PASSABLE(i, j));
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
  SET_FLOOR(y, x);
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
void dungeon_gen_cave(int goal) {
  int i = 1, x=1, y=1;
  // set x, y to be in the middle of allocated dungeon
  // set CURRENT_HEIGHT, CURRENT_WIDTH to be 1
  // set DUNGEON_X, DUNGEON_Y to be x, y
  // update these values as you add more open squares
  // when you hit an edge, shift over the whole dungeon by 128
  // and update all the variables accordingly
  // have a macro to copy dungeon blocks when you do the shift
  while (i < goal) {
    if (DUNGEON[y][x].type == TILE_WALL)
      i++;
    SET_FLOOR(y, x);
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
}

void dungeon_place_items() {
  /* Place items */
  /*  i = 10;
  while (i --> 0) {
    do {
      x = rand_int(0, CURRENT_WIDTH-1);
      y = rand_int(0, CURRENT_HEIGHT-1);
    } while (DUNGEON[y][x].type != TILE_FLOOR);
    item_place(y, x, "diamond");
    }*/
  ;
}

void dungeon_gen(enum DUNGEON_TYPE type) {
  dungeon_clear();
  switch (type) {
  case DUNGEON_MAZE:
    dungeon_gen_maze(1, 1);
    /* place_player() */
    break;
  case DUNGEON_CAVE:
    dungeon_gen_cave(1000);
    break;
  }
  //dungeon_place_items();
  dungeon_set_fov();
}
