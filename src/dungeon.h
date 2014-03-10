#ifndef DUNGEON_H
#define DUNGEON_H

#include "defs.h"

ITEM* get_item(int y, int x);
void dungeon_del();
void dungeon_gen(enum DUNGEON_TYPE type);

#endif
