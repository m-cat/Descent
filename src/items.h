#ifndef ITEMS_H
#define ITEMS_H

#include "models.h"

void    item_copy(ITEM *dest, ITEM *source);
ITEM    *item_create(MODEL_ITEM *model);
ITEM    *item_get_top(unsigned int y, unsigned int x);
ITEM    *item_pickup(unsigned int y, unsigned int x, int i);
void    item_place(unsigned int y, unsigned int x, MODEL_ITEM *model);
void    item_drop(unsigned int y, unsigned int x, ITEM *item);
void    item_delete(ITEM *item);

#endif
