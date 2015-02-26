#ifndef PLAYER_H
#define PLAYER_H

#include <libtcod.h>

extern ACTOR    *player;

void            player_create();
void            player_place(unsigned int y, unsigned int x);

void            handle_scroll(DIRECTION dir);
void            handle_look(DIRECTION dir);
void            handle_inv(DIRECTION dir);
int             attempt_move(DIRECTION dir);
int             handle_direction(DIRECTION dir);

int             handle_input(TCOD_event_t ev, TCOD_keycode_t key, char ch, unsigned int ctrl, unsigned int m_x, unsigned int m_y);

#endif
