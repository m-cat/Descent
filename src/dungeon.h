#ifndef DUNGEON_H
#define DUNGEON_H

#include "defs.h"
#include "models.h"

void            block_copy(DUNGEON_BLOCK *dest, DUNGEON_BLOCK *source);
DUNGEON_BLOCK   *block_create(unsigned int y, unsigned int x, MODEL_BLOCK *model);

void            CLR_OPTS(unsigned int y, unsigned int x);
void            CLR_BLOCK(unsigned int y, unsigned int x);

void            dungeon_clear();
void            dungeon_resize(unsigned int resize_y, unsigned int resize_x, unsigned int shift_up, unsigned int shift_left);
void            dungeon_next();

void            dungeon_set_fov();

DIRECTION       random_valid_path(unsigned int x, unsigned int y, unsigned int maze_x, unsigned int maze_y, unsigned int width, unsigned int height);
void            dungeon_gen_maze(unsigned int x, unsigned int y, unsigned int maze_x, unsigned int maze_y, unsigned int width, unsigned int height);
void            dungeon_gen_cave(unsigned int goal);

// void dungeon_gen_reg(int rm_size_min, int rm_size_max);
void            dungeon_gen(enum DUNGEON_TYPE type);

void            dungeon_place_items();
void            dungeon_place_enemies();

#endif
