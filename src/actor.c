#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "items.h"
#include "util.h"
#include "actor.h"

ACTOR* actor_create(int y, int x, char *name) {
  ACTOR **iterator;
  ACTOR *a;
  for (iterator = (ACTOR**)TCOD_list_begin(actor_type_list);
       iterator != (ACTOR**)TCOD_list_end(actor_type_list);
       iterator++) {
    if (strcmp(name, (*iterator)->name) == 0) {
      a = malloc(sizeof(ACTOR));
      *a = (**iterator);
      a->y = y;
      a->x = x;
      a->inventory = malloc(sizeof(TCOD_list_t));
      *(a->inventory) = TCOD_list_new();
      /* TODO: add possible items to actor's inventory */
      DUNGEON[y][x].resident = a;
      if (strcmp(name, "player") != 0)
	priq_push(actor_queue, a, a->spd);
      return a;
    }
  }
  return NULL;
}

void actor_delete(ACTOR *a) {
  ITEM_N *iterator;

  /* Delete inventory */
  while (TCOD_list_size(*(a->inventory)) > 0) {
    iterator = TCOD_list_pop(*(a->inventory));
    item_delete(iterator->item); /* free the item */
    free(iterator);       /* free the item_n */
  }
  TCOD_list_delete(*(a->inventory));
  free(a->inventory);

  free(a);
}

int can_move(ACTOR *a, int dy, int dx) {
  int x = a->x+dx, y = a->y+dy;
  //FURN *furn = DUNGEON[y][x].furn;
  /* Check for bridge */
  return (CHK_PASSABLE(y, x) && DUNGEON[y][x].resident == NULL);
}

void actor_move(ACTOR *a, int dy, int dx) {
  DUNGEON[a->y][a->x].resident = NULL;
  a->y += dy;
  a->x += dx;
  DUNGEON[a->y][a->x].resident = a;
}

/* Adds item to the actor's inventory */
void actor_add_item(ACTOR *a, ITEM *item) {
  ITEM_N **iterator;
  ITEM_N *item_n;

  for (iterator = (ITEM_N**)TCOD_list_begin(*(a->inventory));
       iterator != (ITEM_N**)TCOD_list_end(*(a->inventory)); iterator++) {
    if (strcmp((*iterator)->item->name, item->name) == 0) {
      (*iterator)->n ++;
      item_delete(item);
      return;
    }
  }

  /* No stack of the same item exists, create new stack */
  item_n = malloc(sizeof(ITEM_N));
  item_n->item = item;
  item_n->n = 1;
  TCOD_list_push(*(a->inventory), (const void *)item_n);  
}

/* Removes the i'th item from the actor's inventory, returning it */
ITEM* actor_get_item_i(ACTOR *a, int i) {
  ITEM_N *item_n;
  ITEM *item;

  if (i == -1)
    item_n = TCOD_list_peek(*(a->inventory));
  else
    item_n = TCOD_list_get(*(a->inventory), i);

  if (item_n->n == 1) {
    item = item_n->item;
    TCOD_list_remove(*(a->inventory), (const void *)item_n);
    free(item_n);
  }
  else {
    item = malloc(sizeof(ITEM));
    item_copy(item, item_n->item);
    item_n->n --;
  }

  return item;
}

/* Picks up the i'th item in the stash */
void actor_pickup(ACTOR *a, int i) {
  ITEM *item = item_pickup(a->y, a->x, i);

  /* Add message if visible */
  if (CHK_VISIBLE(a->y, a->x)) {
    message_add(sentence_form(a->art, 1, a->name, "picks up", "pick up",
			      item->art, 1, item->name), ".");
  }

  actor_add_item(a, item);
}

/* Drops the i'th item in the actor's inventory */
void actor_drop(ACTOR *a, int i) {
  ITEM *item = actor_get_item_i(a, i);
 
  /* Add message if visible */
  if (CHK_VISIBLE(a->y, a->x)) {
    message_add(sentence_form(a->art, 1, a->name, "drops", "drop",
			      item->art, 1, item->name), ".");
  }

  item_drop(a->y, a->x, item);
}

/* Wields the i'th item in the actor's inventory */
void actor_wield(ACTOR *a, int i) {
  ITEM *item;

  item = actor_get_item_i(a, i);
  if (a->weapon != NULL) {
    actor_add_item(a, a->weapon);
  }
  a->weapon = item;
  
  /* Add message if visible */
  if (CHK_VISIBLE(a->y, a->x)) {
    message_add(sentence_form(a->art, 1, a->name, "wields", "wield",
			      item->art, 1, item->name), ".");
  }
}

/* a attacks b */
/* TODO: Implement combat calculations */
void actor_attack(ACTOR *a, ACTOR *b) {
  message_add(sentence_form(a->art, 1, a->name, "attacks", "attack",
			    b->art, 1, b->name), "!");

  
}

void actor_act(ACTOR *a) {
  int dx, dy;
  do {
    dx = rand_int(-1,1), dy = rand_int(-1,1);
  } while (!can_move(a, dy, dx));
  actor_move(a, dy, dx);
}
