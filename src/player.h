#ifndef PLAYER_H
	#define PLAYER_H

	#include <libtcod.h>

extern ACTOR*	player;

void			player_place(int y, int x);
int				handle_input(TCOD_event_t ev, TCOD_keycode_t key, char ch, int ctrl, int m_x, int m_y);
#endif
