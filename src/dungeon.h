#ifndef DUNGEON_H
#define DUNGEON_H

#include "defs.h"
#include "models.h"

void block_copy(DUNGEON_BLOCK *dest, DUNGEON_BLOCK *source);
DUNGEON_BLOCK *block_create(uint y, uint x, MODEL_BLOCK *model);

void CLR_OPTS(uint y, uint x);
void CLR_BLOCK(uint y, uint x);

void dungeon_clear();
void dungeon_resize(uint resize_y, uint resize_x, uint shift_up,
                    uint shift_left);
void dungeon_next();

void dungeon_set_fov();

DIRECTION
random_valid_path(uint x, uint y, uint maze_x, uint maze_y, uint width,
                  uint height);
void dungeon_gen_maze(uint x, uint y, uint maze_x, uint maze_y, uint width,
                      uint height);
void dungeon_gen_cave(uint goal);

// void dungeon_gen_reg(int rm_size_min, int rm_size_max);
void dungeon_gen(enum DUNGEON_TYPE type);

void dungeon_place_items();
void dungeon_place_enemies();

#endif
