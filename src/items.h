#ifndef ITEMS_H
#define ITEMS_H

#include "models.h"

void item_copy(ITEM *dest, ITEM *source);
ITEM *item_create(MODEL_ITEM *model);
ITEM *item_get_top(uint y, uint x);
ITEM *item_pickup(uint y, uint x, int i);
void item_place(uint y, uint x, MODEL_ITEM *model);
void item_drop(uint y, uint x, ITEM *item);
void item_delete(ITEM *item);

#endif
