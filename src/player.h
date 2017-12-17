#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include <libtcod.h>

extern ACTOR *player;

void player_create();
void player_place(int y, int x);

void handle_scroll(DIRECTION dir);
void handle_look(DIRECTION dir);
void handle_inv(DIRECTION dir);
int try_move(DIRECTION dir);
int handle_direction(DIRECTION dir);

int handle_input(TCOD_event_t ev, TCOD_keycode_t key, char ch, int ctrl,
                 int shift, int m_x, int m_y);

#endif
