#ifndef ACTOR_H
#define ACTOR_H

#include "defs.h"

ACTOR	*actor_create(int y, int x, char *name);
void	actor_delete(ACTOR *a);

void	advance_turn();

int		can_move(ACTOR *a, int dy, int dx);
void	actor_move(ACTOR *a, int dy, int dx);
void	actor_pickup(ACTOR *a, int i);
void	actor_drop(ACTOR *a, int i);
void	actor_wield(ACTOR *a, int i);

void	actor_attack(ACTOR *a, ACTOR *b);
void	actor_act(ACTOR *a);
#endif
