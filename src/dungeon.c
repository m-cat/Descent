#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "priority.h"
#include "items.h"
#include "actor.h"
#include "player.h"
#include "dungeon.h"

/* Use this to make a copy of the block's resources.
   Usually doing dest = source is fine, though. */
void block_copy(DUNGEON_BLOCK *dest, DUNGEON_BLOCK *source) {
  *dest = *source;
  dest->name = strdup(source->name);
  dest->art = strdup(source->art);
}

void block_create(int y, int x, char *name) {
  DUNGEON_BLOCK **iterator;
  for (iterator = (DUNGEON_BLOCK**)TCOD_list_begin(block_type_list);
       iterator != (DUNGEON_BLOCK**)TCOD_list_end(block_type_list);
       iterator++) {
    if (strcmp(name, (*iterator)->name) == 0) {
      DUNGEON[y][x] = (**iterator);
      return;
    }
  }
}

/* Due to how frequently walls and floors are, placed, it's
   prudent to circumvent the inefficient block_create() function. */
void SET_WALL(int y, int x) {
  DUNGEON[y][x] = block_wall;
}
void SET_FLOOR(int y, int x) {
  DUNGEON[y][x] = block_floor;
}

void CLR_OPTS(int y, int x) {
  DUNGEON[y][x].PASSABLE = 0;
  DUNGEON[y][x].TRANSPARENT = 0;
  DUNGEON[y][x].VISIBLE = 0;
  DUNGEON[y][x].EXPLORED = 0;
}
void CLR_BLOCK(int y, int x) {
  ITEM_N *iterator;
  DUNGEON_BLOCK *block = &DUNGEON[y][x];

  assert(block != NULL);
  CLR_OPTS(y, x);
  block->name = NULL;
  block->art = NULL;
  block->resident = NULL; /* Don't free the resident here;
			     we do it in dungeon_clear(). */
  if (block->furn != NULL) {
    free(block->furn);
    block->furn = NULL;
  }
  /* Delete every item in a stash */
  if (block->stash != NULL) {
    while (TCOD_list_size(*(block->stash)) > 0) {
      iterator = TCOD_list_pop(*(block->stash));
      item_delete(iterator->item); /* free the item */
      free(iterator);       /* free the item_n */
    }
    TCOD_list_delete(*(block->stash));
    free(block->stash);
  }
}

/* Clear all data from every dungeon block.
   Set the dungeon to be all walls. */
void dungeon_clear() {
  int i, j, pri;
  ACTOR *a;

  /* Clear all blocks */
  for (i = 0; i < MAX_HEIGHT; i++) {
    for (j = 0; j < MAX_WIDTH; j++) {
      CLR_BLOCK(i, j);
      SET_WALL(i, j);
    }
  }

  /* Delete all actors */
  while ((a = priq_pop(actor_queue, &pri))) {
    /* Take out actor from the queue and delete it*/
    actor_delete(a);
  }

  /* Delete the fov map */
  TCOD_map_delete(fov_map);
}

/* Resizes a dungeon by amount ABS(resize_x), ABS(resize_y).
   If resize_x or resize_y are negative, the dungeon will be
   resized in the negative direction - that is, elements
   will be shifted over to simulate a negatively-expanded grid. */
void dungeon_resize(int resize_x, int resize_y) {
  int i, j;
  int prev_height = MAX_HEIGHT, prev_width = MAX_WIDTH;
  int abs_resize_y = ABS(resize_y), abs_resize_x = ABS(resize_x);

  if (resize_y < 0)
    DUNGEON_Y += abs_resize_y;
  if (resize_x < 0)
    DUNGEON_X += abs_resize_x;

  if (resize_y) {
    MAX_HEIGHT += abs_resize_y;
    DUNGEON = realloc(DUNGEON, MAX_HEIGHT * sizeof(DUNGEON_BLOCK *));
    /* If resize_y < 0, shift over all the rows */
    if (resize_y < 0) {
      for (i = prev_height-1; i >= 0; i--)
	DUNGEON[i+abs_resize_y] = DUNGEON[i];
      for (i = abs_resize_y-1; i >= 0; i--) {
	DUNGEON[i] = calloc(MAX_WIDTH, sizeof(DUNGEON_BLOCK));
	for (j = 0; j < MAX_WIDTH; j++)
	  SET_WALL(i, j);
      }
    }
    else {
      for (i = prev_height; i < MAX_HEIGHT; i++) {
	DUNGEON[i] = calloc(MAX_WIDTH, sizeof(DUNGEON_BLOCK));
	for (j = 0; j < MAX_WIDTH; j++)
	  SET_WALL(i, j);
      }
    }
  }

  if (resize_x) {
    MAX_WIDTH += abs_resize_x;
    for (i = 0; i < MAX_HEIGHT; i++) {
      DUNGEON[i] = realloc(DUNGEON[i], MAX_WIDTH * sizeof(DUNGEON_BLOCK));
      if (resize_x < 0) {
	/* Shift over all the columns */
	for (j = prev_width-1; j >= 0; j--)
	  DUNGEON[i][j+abs_resize_x] = DUNGEON[i][j];
	for (j = abs_resize_x-1; j >= 0; j--) {
	  DUNGEON[i][j].furn = NULL; /* Since this dungeon block has the
					old values, we need to prevent
					CLR_BLOCK() from calling free()
					on an existing furn. */
	  DUNGEON[i][j].stash = NULL; /* Same deal here. */
	  CLR_BLOCK(i, j);
	  SET_WALL(i, j);
	}
      }
      else {
	for (j = prev_width; j < MAX_WIDTH; j++) {
	  DUNGEON[i][j].furn = NULL; /* This dungeon block will have
					garbage values; we need to 
					prevent CLR_BLOCK() from calling
					free() on them. */
	  DUNGEON[i][j].stash = NULL;
	  CLR_BLOCK(i, j);
	  SET_WALL(i, j);
	}
      }
    }
  }
}

void dungeon_set_fov() {
  int i, j;
  fov_map = TCOD_map_new(MAX_WIDTH, MAX_HEIGHT);
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
  block_create(y, x, "downward staircase");

  while (i < goal) {
    switch (rand_int(DIR_N, DIR_W)) {
    case DIR_N:
      y -= 1;
      break;
    case DIR_E:
      x += 1;
      break;
    case DIR_S:
      y += 1;
      break;
    case DIR_W:
      x -= 1;
      break;
    default:
      break;
    }
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
      
      /* When we hit an edge, we shift over the whole dungeon by 128
	 and update all the variables accordingly. */
      resize_x = 128*(DUNGEON_X + CURRENT_WIDTH >= MAX_WIDTH - 1) -
	128*(DUNGEON_X <= 1);
      resize_y = 128*(DUNGEON_Y + CURRENT_HEIGHT >= MAX_HEIGHT - 1) -
	128*(DUNGEON_Y <= 1);
      if (resize_x || resize_y) {
	dungeon_resize(resize_x, resize_y); /* expand dungeon size in memory */
	if (resize_y < 0)
	  y += ABS(resize_y);
	if (resize_x < 0)
	  x += ABS(resize_x);
      }
      SET_FLOOR(y, x);
    }
  }
  block_create(y, x, "upward staircase");
  player_place(y, x);
}

typedef struct {
  int x1, x2;
  int y1, y2;
  enum DIRECTION dir;
} DUNGEON_WALL;

void dungeon_gen_reg(int rm_size_min, int rm_size_max) {
  TCOD_list_t wall_list = TCOD_list_allocate(100);

  

  TCOD_list_delete(wall_list);
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
  item_place(y, x, "pickaxe");
}

void dungeon_place_enemies() {
  /* Place enemies */
  int i = 50, x, y;
  while (i --> 0) {
    do {
      x = rand_int(DUNGEON_X+1, DUNGEON_X+CURRENT_WIDTH-1);
      y = rand_int(DUNGEON_Y+1, DUNGEON_Y+CURRENT_HEIGHT-1);
    } while (DUNGEON[y][x].type != TILE_FLOOR);
    actor_create(y, x, rand_int(0,1) ? "dungeon rat" : "dungeon rat");
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
  dungeon_place_enemies();
  dungeon_set_fov();
}

void dungeon_next() {
  DEPTH++;
  dungeon_gen(DUNGEON_CAVE);
  message_add(string_create(1, "You descend down the stairs"), "!");
}
