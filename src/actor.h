#ifndef ACTOR_H
#define ACTOR_H

#include "defs.h"
#include "models.h"

ACTOR *actor_create(uint y, uint x, MODEL_ACTOR *model);
void actor_free(ACTOR *a);

int actor_can_move(ACTOR *a, uint y, uint x);
void actor_try_move(ACTOR *a, uint y, uint x);

void actor_add_item(ACTOR *a, ITEM *item);
ITEM *actor_get_item_i(ACTOR *a, int i);
void actor_pickup(ACTOR *a, int i);
void actor_drop(ACTOR *a, int i);
void actor_wield(ACTOR *a, int i);

void actor_attack(ACTOR *a, ACTOR *b);
void actor_act(ACTOR *a);
#endif
