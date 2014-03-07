#ifndef ACTOR_H
#define ACTOR_H

#include "defs.h"

int can_move(ACTOR *a, enum DIRECTION d);
void actor_move(ACTOR *a, enum DIRECTION d);

#endif
