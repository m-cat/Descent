#ifndef ITEMS_H
#define ITEMS_H

void	item_copy(ITEM *dest, ITEM *source);
ITEM	*item_create(char *name);
ITEM	*item_get_top(int y, int x);
ITEM	*item_pickup(int y, int x, int i);
void	item_place(int y, int x, char *name);
void	item_drop(int y, int x, ITEM *item);
void	item_delete(ITEM *item);
#endif
