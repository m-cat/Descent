#include <stdlib.h>
#include <assert.h>
#include "defs.h"
#include "util.h"
#include "items.h"
#include "player.h"
#include "dungeon.h"

void SET_WALL(int y, int x) {
  DUNGEON[y][x].type = TILE_WALL;
  DUNGEON[y][x].ch = '#';
  DUNGEON[y][x].name = "wall";
  SET_TRANSPARENT(y, x, 0);
  SET_PASSABLE(y, x, 0);
}
void SET_FLOOR(int y, int x) {
  DUNGEON[y][x].type = TILE_FLOOR;
  DUNGEON[y][x].ch = '.';
  DUNGEON[y][x].name = "floor";
  SET_TRANSPARENT(y, x, 1);
  SET_PASSABLE(y, x, 1);
}
void SET_STAIRS_DOWN(int y, int x) {
  DUNGEON[y][x].type = TILE_STAIRS_DOWN;
  DUNGEON[y][x].ch = '>';
  DUNGEON[y][x].name = "downward staircase";
  SET_TRANSPARENT(y, x, 1);
  SET_PASSABLE(y, x, 1);
}
void CLR_OPTS(int y, int x) {
  DUNGEON[y][x].PASSABLE = 0;
  DUNGEON[y][x].TRANSPARENT = 0;
  DUNGEON[y][x].VISIBLE = 0;
  DUNGEON[y][x].EXPLORED = 0;
}

/* Set the dungeon to be all walls */
void dungeon_clear() {
  int i, j;
  DUNGEON_BLOCK *block;
  ITEM_N **iterator;

  /* Clear all blocks */
  for (i = 0; i < MAX_HEIGHT; i++) {
    for (j = 0; j < MAX_WIDTH; j++) {
      block = &DUNGEON[i][j];
      assert(block != NULL);
      CLR_OPTS(i, j);
      SET_WALL(i, j);
      block->resident = NULL;
      if (block->furn != NULL) {
	free(block->furn);
	block->furn = NULL;
      }

      /* Delete every item in a stash */
      if (block->stash != NULL) {
	for (iterator = (ITEM_N**)TCOD_list_begin(*(block->stash));
	     iterator != (ITEM_N**)TCOD_list_end(*(block->stash)); iterator++) {
	  item_delete((*iterator)->item); /* free the item */
	  free((*iterator));       /* free the item_n */
	}
	TCOD_list_delete(block->stash);
	free(block->stash);
	block->stash = NULL;
      }
    }
  }

  /* Delete all actors */

  /* Clear fov map for dungeon */
  TCOD_map_clear(fov_map, 0, 0);
}

void dungeon_set_fov() {
  int i, j;
  for (i = DUNGEON_Y; i < DUNGEON_Y+CURRENT_HEIGHT; i++)
    for (j = DUNGEON_X; j < DUNGEON_X+CURRENT_WIDTH; j++)
      TCOD_map_set_properties(fov_map, j, i,
			      CHK_TRANSPARENT(i, j),
			      CHK_PASSABLE(i, j));
}

enum DIRECTION random_valid_path(int x, int y,
				 int maze_x, int maze_y, int width, int height) {
  enum DIRECTION paths[4];
  int count = 0;
  if (y > maze_y+1 && DUNGEON[y-2][x].type == TILE_WALL)
    paths[count++] = DIR_N;
  if (x < maze_x+width-3 && DUNGEON[y][x+2].type == TILE_WALL)
    paths[count++] = DIR_E;
  if (y < maze_y+height-3 && DUNGEON[y+2][x].type == TILE_WALL)
    paths[count++] = DIR_S;
  if (x > maze_x+1 && DUNGEON[y][x-2].type == TILE_WALL)
    paths[count++] = DIR_W;
  return count ? paths[rand_int(0,count-1)] : DIR_NONE;
}

/* Generates a maze within the specified boundaries.
   width and height should be odd numbers. */
void dungeon_gen_maze(int x, int y,
		      int maze_x, int maze_y, int width, int height) {
  enum DIRECTION d;
  SET_FLOOR(y, x);
  while ((d = random_valid_path(x, y, maze_x, maze_y, width, height))
	 != DIR_NONE) {
    switch (d) {
    case DIR_N:
      SET_FLOOR(y-1, x);
      dungeon_gen_maze(x, y-2, maze_x, maze_y, width, height);
      break;
    case DIR_E:
      SET_FLOOR(y, x+1);
      dungeon_gen_maze(x+2, y, maze_x, maze_y, width, height);
      break;
    case DIR_S:
      SET_FLOOR(y+1, x);
      dungeon_gen_maze(x, y+2, maze_x, maze_y, width, height);
      break;
    case DIR_W:
      SET_FLOOR(y, x-1);
      dungeon_gen_maze(x-2, y, maze_x, maze_y, width, height);
      break;
    default:
      break;
    }
  }
}
void dungeon_gen_cave(int goal) {
  int i = 1, x=MAX_WIDTH/2, y=MAX_HEIGHT/2, resize_x, resize_y;
  CURRENT_HEIGHT = CURRENT_WIDTH = 3;
  DUNGEON_X = x-1, DUNGEON_Y = y-1;
  SET_STAIRS_DOWN(y, x);
  // set x, y to be in the middle of allocated dungeon
  // set CURRENT_HEIGHT, CURRENT_WIDTH to be 1
  // set DUNGEON_X, DUNGEON_Y to be x, y
  // update these values as you add more open squares
  // when you hit an edge, shift over the whole dungeon by 128
  // and update all the variables accordingly
  // have a macro to copy dungeon blocks when you do the shift
  // Remember to resize the fov_map too.
  while (i < goal) {
    if (DUNGEON[y][x].type == TILE_WALL) {
      i++;
      resize_x = resize_y = 0;
      if (y <= DUNGEON_Y) {
	DUNGEON_Y -= 1;
	CURRENT_HEIGHT += 1;
      }
      else if (y >= DUNGEON_Y + CURRENT_HEIGHT - 1)
	CURRENT_HEIGHT += 1;
      if (x <= DUNGEON_X) {
	DUNGEON_X -= 1;
	CURRENT_WIDTH += 1;
      }
      else if (x >= DUNGEON_X + CURRENT_WIDTH - 1)
	CURRENT_WIDTH += 1;
      /*
      resize_x = 128*(DUNGEON_X + CURRENT_WIDTH >= MAX_WIDTH - 1) -
      128*(DUNGEON_X <= 1);
      resize_y = 128*(DUNGEON_Y + CURRENT_HEIGHT >= MAX_HEIGHT - 1) -
      128*(DUNGEON_Y <= 1);
      if (resize_x || resize_y)
	dungeon_resize(resize_x, resize_y);
      if (resize_y < 0)
	y -= resize_y;
      if (resize_x < 0)
      x -= resize_x;*/
      SET_FLOOR(y, x);
    }
    switch (rand_int(DIR_N, DIR_W)) {
    case DIR_N:
      y -= (y>1);
      break;
    case DIR_E:
      x += (x < MAX_WIDTH-2);
      break;
    case DIR_S:
      y += (y < MAX_HEIGHT-2);
      break;
    case DIR_W:
      x -= (x>1);
      break;
    default:
      break;
    }
  }
  SET_FLOOR(y, x);
  player_place(y, x);
}

void dungeon_place_items() {
  /* Place items */
  int i = 12, x, y;
  while (i --> 0) {
    do {
      x = rand_int(DUNGEON_X+1, DUNGEON_X+CURRENT_WIDTH-1);
      y = rand_int(DUNGEON_Y+1, DUNGEON_Y+CURRENT_HEIGHT-1);
    } while (DUNGEON[y][x].type != TILE_FLOOR);
    item_place(y, x, rand_int(0,1) ? "diamond" : "apple");
    }  
}

void dungeon_gen(enum DUNGEON_TYPE type) {
  dungeon_clear();
  switch (type) {
  case DUNGEON_MAZE:
    DUNGEON_X = DUNGEON_Y = 2;
    CURRENT_WIDTH = CURRENT_HEIGHT = 11;
    dungeon_gen_maze(3, 3, DUNGEON_X, DUNGEON_Y, CURRENT_WIDTH, CURRENT_HEIGHT);
    player_place(3, 3);
    break;
  case DUNGEON_CAVE:
    dungeon_gen_cave(1000);
    break;
  case DUNGEON_REGULAR:
    //dungeon_gen_reg();
    break;
  default:
    break;
  }
  dungeon_place_items();
  dungeon_set_fov();
}
