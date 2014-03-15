#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "items.h"
#include "util.h"
#include "priority.h"
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
      DUNGEON[y][x].resident = a;
      if (strcmp(name, "player") != 0)
	priq_push(enemy_queue, a, a->spd);
      return a;
    }
  }
  return NULL;
}

/* Advances a turn for ALL actors apart from player. */
void advance_turn() {
  ACTOR *a;
  int pri;
  while ((a = priq_pop(enemy_queue, &pri))) {
    /* Take out actor from the queue, act, put it in a temp queue */
    actor_act(a);
    priq_push(temp_queue, a, pri);
  }
  /* Put all actors in the temp queue back into the main queue */
  priq_combine(enemy_queue, temp_queue);
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

/* Picks up the i'th item in the stash located at the given coordinates */
void actor_pickup(ACTOR *a, int y, int x, int i) {
  ITEM_N **iterator;
  ITEM_N *item_n;
  ITEM *item = item_pickup(y, x, i);

  /* Add message if visible */
  if (CHK_VISIBLE(y, x)) {
    message_add(sentence_form(a->art, 1, a->name, "picks up", "pick up",
			      item->art, 1, item->name), ".");
  }

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

/* Drops the i'th item in the actor's inventory */
void actor_drop(ACTOR *a, int y, int x, int i) {
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
  item_drop(y, x, item);
}

void actor_act(ACTOR *a) {
  ;
}
