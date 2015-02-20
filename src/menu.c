#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "io.h"
#include "player.h"
#include "menu.h"

/* Define menu variables */
int MENU_CHOICE = 0; /* persists when user returns to menu */
const int MENU_NUM_CHOICES = 2;

enum MENU_SCREEN {
  MENU_MAIN,
  MENU_NAME,
};


int draw_menu(enum MENU_SCREEN menu) {
  char *str, *temp;
  int cur_len, max_len;
  TCOD_key_t key;
  switch (menu) {
  case MENU_MAIN:
    cprint(1, 1, TCOD_white, TCOD_black, "  New Game");
    cprint(2, 1, TCOD_white, TCOD_black, "  Quit");
    cprint(MENU_CHOICE+1, 1, TCOD_white, TCOD_black, ">");
    TCOD_console_flush();
    break;
  case MENU_NAME:
#define OFFSET_NAME_X 2
#define OFFSET_NAME_Y 3
    cur_len = 0, max_len = MAX_NAME_LEN;
    str = calloc(max_len+1, 1);

    do {
      TCOD_console_clear(NULL);
      cprint(OFFSET_NAME_Y, OFFSET_NAME_X, TCOD_white, TCOD_black, "> %s_", str);
      cprint(OFFSET_NAME_Y, OFFSET_NAME_X+max_len+1, TCOD_white, TCOD_black, " "); /* erase '_' if at end */
      cprint(1, 1, TCOD_white, TCOD_black, "Welcome, adventurer! What is your name?");
      cprint(5, 1, TCOD_white, TCOD_black, "Press space for a random name.");
      TCOD_console_flush();

      TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS,&key,NULL,1);
	  if (TCOD_console_is_window_closed()) {
		game_end();
	  }

      if ((is_alphanum(key.c) || key.c == '-') && cur_len < max_len-1) {
		str[cur_len++] = key.c;
		str[cur_len] = 0;
      }
      else if ((key.vk == TCODK_BACKSPACE || key.vk == TCODK_DELETE) && cur_len > 0)
		str[--cur_len] = 0;
      else if (key.c == ' ') {
		temp = name_gen();
		cur_len = strlen(strcpy(str, temp));
		free(temp);
      }
      else if (key.vk == TCODK_ESCAPE) {
		free(str);
		return -1;
      }
    } while ((key.vk != TCODK_ENTER && key.vk != TCODK_KPENTER) || cur_len == 0);
    PLAYER_NAME = str;
    break;
  }
  return 0;
}

enum {
  MENU_NEW,
  MENU_QUIT,
};

int handle_menu() {
  TCOD_key_t key;

 main_menu:

  TCOD_console_clear(NULL);
  draw_menu(MENU_MAIN);
  /* Initial menu screen */
  do {
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS,&key,NULL,1);
	if (TCOD_console_is_window_closed()) {
		game_end();
	}
    switch (key.vk) {
    case TCODK_UP:
      MENU_CHOICE = (MENU_CHOICE == 0) ? MENU_NUM_CHOICES-1 : MENU_CHOICE-1;
      break;
    case TCODK_DOWN:
      MENU_CHOICE = (MENU_CHOICE == MENU_NUM_CHOICES-1) ? 0 : MENU_CHOICE+1;
      break;
    case TCODK_ESCAPE:
      return 1;
      break;
    default:
      break;
    }
    draw_menu(MENU_MAIN);
  } while (key.vk != TCODK_ENTER && key.vk != TCODK_KPENTER && key.c != ' ');

  TCOD_console_clear(NULL);

  /* QUIT */
  if (MENU_CHOICE == MENU_QUIT)
    return 1;

  /* NEW GAME */
  if (draw_menu(MENU_NAME) == -1)
    goto main_menu;

  TCOD_console_clear(NULL);

  return 0;
}
