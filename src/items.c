#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "items.h"

void item_init(ITEM *item, char *name) {
  /*  FILE *fp;
  size_t len = 40;
  char *line = malloc(len + 1);

  fp = fopen("data/items", "r");
  if (fp == NULL) {
    fclose(fp);
    fprintf(stderr, "Could not find data/items file.\n");
    exit(1);
  }

  while (getline2(line, &len, fp) != -1) {
    if (strcmp(line, name) == 0) {
      item->name = name;
      getline2(line, &len, fp);
      item->type = atoi(line);
      getline2(line, &len, fp);
      item->ch = line[0];

      fclose(fp);
      return;
    }
  }

  fclose(fp);
  fprintf(stderr, "Error reading data/items file.\n");
  exit(1);*/
  ;
}

ITEM* item_get(int y, int x) {
  if (DUNGEON[y][x].items == NULL)
    return NULL;
  return DUNGEON[y][x].items->item;
}

void item_place(int y, int x, char* name) {
  ITEM *item = malloc(sizeof(ITEM));
  item_init(item, name);
  item_drop(y, x, item);
}
void item_drop(int y, int x, ITEM *item) {
  ITEM_STACK *items = DUNGEON[y][x].items;
  ITEM_STACK *new_items = malloc(sizeof(ITEM_STACK));
  new_items->item = item;
  new_items->next = items;
  if (items != NULL)
    items->prev = new_items;
  new_items->prev = NULL;
  DUNGEON[y][x].items = new_items;
}
