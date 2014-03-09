#ifndef ITEMS_H
#define ITEMS_H

void item_init(ITEM *item, char *name);

ITEM* item_get(int y, int x);

void item_place(int y, int x, char* name);

void item_drop(int y, int x, ITEM *item);

#endif
