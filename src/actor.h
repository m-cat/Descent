#ifndef ACTOR_H
#define ACTOR_H

#include "defs.h"
#include "models.h"

ACTOR   *actor_create(unsigned int y, unsigned int x, MODEL_ACTOR *model);
void    actor_delete(ACTOR *a);

int     actor_can_move(ACTOR *a, unsigned int y, unsigned int x);
void    actor_attempt_move(ACTOR *a, unsigned int y, unsigned int x);

void    actor_add_item(ACTOR *a, ITEM *item);
ITEM    *actor_get_item_i(ACTOR *a, int i);
void    actor_pickup(ACTOR *a, int i);
void    actor_drop(ACTOR *a, int i);
void    actor_wield(ACTOR *a, int i);

void    actor_attack(ACTOR *a, ACTOR *b);
void    actor_act(ACTOR *a);
#endif
