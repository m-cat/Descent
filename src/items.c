#include <stdlib.h>
#include <string.h>
#include <libtcod.h>
#include <assert.h>
#include "defs.h"
#include "io.h"
#include "items.h"

void item_copy(ITEM *dest, ITEM *source) {
  *dest = *source;
  dest->name = strdup(source->name);
  dest->art = strdup(source->art);
}

ITEM* item_create(char *name) {
  ITEM **iterator;
  ITEM *item;
  for (iterator = (ITEM**)TCOD_list_begin(item_type_list);
       iterator != (ITEM**)TCOD_list_end(item_type_list); iterator++) {
    if (strcmp(name, (*iterator)->name) == 0) {
      item = malloc(sizeof(ITEM));
      item_copy(item, (*iterator));
      return item;
    }
  }
  return NULL;
}

/* Returns a pointer to the top stack's item.
   Does NOT remove the item from the stash */
ITEM* item_get_top(int y, int x) {
  ITEM_N *item_n;
  if (DUNGEON[y][x].stash == NULL || TCOD_list_is_empty(*(DUNGEON[y][x].stash)))
    return NULL;
  item_n = (ITEM_N*)TCOD_list_peek(*(DUNGEON[y][x].stash));
  return item_n->item;
}

/* Removes and returns an item from position i in the stash */
ITEM* item_pickup(int y, int x, int i) {
  ITEM *item;
  ITEM_N *item_n;

  if (i == -1)
    item_n = (ITEM_N*)TCOD_list_peek(*(DUNGEON[y][x].stash));
  else
    item_n = (ITEM_N*)TCOD_list_get(*(DUNGEON[y][x].stash), i);
  item = malloc(sizeof(ITEM));
  item_copy(item, item_n->item);
  if (item_n->n > 1)
    item_n->n --;
  else {
    TCOD_list_remove(*(DUNGEON[y][x].stash), (const void *)item_n);
    item_delete(item_n->item);
    free(item_n);
  }
  return item;
}

void item_drop(int y, int x, ITEM *item) {
  ITEM_N *item_n;
  ITEM_N **iterator;
  TCOD_list_t *stash = DUNGEON[y][x].stash;

  if (stash == NULL) {
    stash = malloc(sizeof(TCOD_list_t));
    *stash = TCOD_list_allocate(1);
  }
  else {
    /* Search for preexisting stack of the same item */
    for (iterator = (ITEM_N**)TCOD_list_begin(*stash);
	 iterator != (ITEM_N**)TCOD_list_end(*stash); iterator++) {
      if (strcmp((*iterator)->item->name, item->name) == 0) {
	(*iterator)->n ++;
	item_delete(item);
	return;
      }
    }
  }
  
  /* No stack of the same item exists, create new stack */
  item_n = malloc(sizeof(ITEM_N));
  item_n->item = item;
  item_n->n = 1;
  TCOD_list_push(*stash, (const void *)item_n);

  DUNGEON[y][x].stash = stash;
}

void item_place(int y, int x, char* name) {
  ITEM *item = item_create(name);
  /* TODO: add error message if item not found */
  assert (item != NULL);
  item_drop(y, x, item);
}

void item_delete(ITEM *item) {
  assert (item != NULL);
  assert (item->name != NULL);
  free(item->name);
  free(item->art);
  free(item);
}
