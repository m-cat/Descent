#ifndef ACTOR_H
#define ACTOR_H

#include "defs.h"

int can_move(actor *a, enum DIRECTION d);
void actor_move(actor *a, enum DIRECTION d);
enum COLOR_PAIR get_actor_color(actor *a);

#endif
