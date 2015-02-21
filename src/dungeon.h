#ifndef DUNGEON_H
	#define DUNGEON_H

	#include "defs.h"

void	block_copy(DUNGEON_BLOCK* dest, DUNGEON_BLOCK* source);

ITEM*	get_item(int y, int x);
void	dungeon_del();
void	dungeon_gen(enum DUNGEON_TYPE type);
void	dungeon_next();
#endif
