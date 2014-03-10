#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "items.h"
#include "player.h"
#include "io.h"

/* Define menu variables */
int MENU_CHOICE = 0; /* persists when user returns to menu */
const int MENU_NUM_CHOICES = 2;

/* Define macros */
#define cprint(y, x, c1, c2, args...)			\
  (assert((x) >= 0), assert((y) >= 0),			\
   assert((x) < CON_WIDTH), assert((y) < CON_HEIGHT),	\
   TCOD_console_set_default_foreground(NULL, c1),	\
   TCOD_console_set_default_background(NULL, c2),	\
   TCOD_console_print(NULL, x, y, args))
#define cprint_center(y, x, c1, c2, args...)		\
  (TCOD_console_set_alignment(NULL, TCOD_CENTER),	\
   cprint(y, x, c1, c2, args),				\
   TCOD_console_set_alignment(NULL, TCOD_LEFT))
#define cprint_right(y, x, c1, c2, args...)		\
  (TCOD_console_set_alignment(NULL, TCOD_RIGHT),	\
   cprint(y, x, c1, c2, args),				\
   TCOD_console_set_alignment(NULL, TCOD_LEFT))

/* isalnum() does not work properly with ncurses keys */
int is_alphanum(int c) {
  return ((c >= 'a' && c <= 'z') ||
	  (c >= 'A' && c <= 'Z') ||
	  (c >= '0' && c <= '9'));
}

TCOD_color_t get_actor_color(ACTOR *a) {
  switch (a->type) {
  case ACTOR_PLAYER:
    return TCOD_white;
  case ACTOR_ENEMY:
    return TCOD_red;
  case ACTOR_NPC:
    return TCOD_lime;
  }
}

void draw_notify(int scr_width) {
  int j;
  ITEM_STACK *items;
  ITEM *item;
  items = DUNGEON[player.y][player.x].items;
  if (items != NULL && items->item != NULL) {
    item = items->item;
    cprint(0, 1, TCOD_white, TCOD_black, " You see here %s %s. %s",
	    a_or_an(item->name), item->name,
	    (items->next != NULL) ? "(more) " : "");
  }
}

void draw_view(int scr_width, int scr_height) {
  int i, j, drawi, drawj;
  DUNGEON_BLOCK *block;
  ACTOR *a;
  FURN *furn;
  ITEM *item;
  enum TILE_TYPE type;
  TCOD_color_t c2; // background color
  for (i = player.y-scr_height/2; i < player.y+scr_height/2; i++)
    for (j = player.x-scr_width/2; j < player.x+scr_width/2; j++) {
      drawi = i-player.y+scr_height/2, drawj = j-player.x+scr_width/2;
      /* Check if out of bounds */
      if (i < 0 || i >= CURRENT_HEIGHT || j < 0 || j >= CURRENT_WIDTH) {
	cprint(drawi, drawj, TCOD_white, TCOD_black, " ");
	continue;
      }
      block = &DUNGEON[i][j];
      assert (block != NULL);
      /* Check if not explored */
      if (!CHK_EXPLORED(i, j)) {
	cprint(drawi, drawj, TCOD_white, TCOD_black, " ");
	continue;
      }
      /* Check if not visible */
      if (!CHK_VISIBLE(i, j)) {
	if (block->type == TILE_WALL)
	  cprint(drawi, drawj, TCOD_sepia, TCOD_black, "#");
	else
	  cprint(drawi, drawj, TCOD_sepia, TCOD_black, ".");
	continue;
      }
      /* Draw the contents of the block
	 Prioritizes actor > furn > item > tile */
      a = block->resident;
      furn = block->furn;
      type = block->type;
      item = item_get(i, j);
      switch (type) {
      case TILE_WATER: c2 = TCOD_blue; break;
      case TILE_LAVA: c2 = TCOD_red; break;
      default: c2 = TCOD_black; break;
      }
      if (a != NULL)
	cprint(drawi, drawj, get_actor_color(a), c2, "%c", a->ch);
      else if (furn != NULL)
	cprint(drawi, drawj, TCOD_yellow, c2, "%c", furn->ch);
      else if (item != NULL)
	cprint(drawi, drawj,
	       TCOD_color_equals(c2,TCOD_blue) ? TCOD_black : TCOD_blue, c2,
	       "%c", item->ch);
      else if (block->type == TILE_FLOOR)
	cprint(drawi, drawj, TCOD_white, TCOD_black, ".");
      else if (block->type == TILE_WALL)
	cprint(drawi, drawj, TCOD_white, TCOD_black, "#");
      else if (block->type == TILE_WATER)
	cprint(drawi, drawj, TCOD_blue, TCOD_black, "~");
      else if (block->type == TILE_WATER)
	cprint(drawi, drawj, TCOD_red, TCOD_black, "~");
    }
}

void draw_ui(int ui_x, int ui_y) {
  int i, j;

  /* Draw separator and borders */
  for (i = 0; i < CON_HEIGHT; i++)
    cprint(i, ui_x, TCOD_black, TCOD_white, " ");

  /* Draw name */
  for (j = ui_x+2; j <= CON_WIDTH-2; j++)
    cprint(ui_y+1, j, TCOD_lime, TCOD_black, "~");
  cprint_center(ui_y+1, ui_x+UI_WIDTH/2+1,
	  TCOD_white, TCOD_black, " %s ", player.name);

  /* Draw level and exp */
  cprint(ui_y+3, ui_x+3, TCOD_white, TCOD_black, "Level: %d", player.level);
  cprint_right(ui_y+3, CON_WIDTH-3, TCOD_white, TCOD_black,
	  "Exp: %d", player.exp);

  /* Draw hp and mp */
  cprint(ui_y+5, ui_x+10, TCOD_white, TCOD_black, "%d / %d", 12, 32);
  cprint(ui_y+6, ui_x+3, TCOD_white, TCOD_black, "HP: [");
  for (j = ui_x+8; j < CON_WIDTH-3; j++)
    cprint(ui_y+6, j, TCOD_flame, TCOD_black, "=");
  cprint(ui_y+6, CON_WIDTH-3, TCOD_white, TCOD_black, "]");

  cprint(ui_y+8, ui_x+10, TCOD_white, TCOD_black, "%d / %d", 5, 5);
  cprint(ui_y+9, ui_x+3, TCOD_white, TCOD_black, "MP: [");
  for (j = ui_x+8; j < CON_WIDTH-3; j++)
    cprint(ui_y+9, j, TCOD_azure, TCOD_black, "=");
  cprint(ui_y+9, CON_WIDTH-3, TCOD_white, TCOD_black, "]");

  /* Draw weapon name */
}

void draw_game() {
  int scr_width = CON_WIDTH-UI_WIDTH;
  int scr_height = CON_HEIGHT;
  TCOD_key_t key;
  draw_view(scr_width, scr_height);
  draw_ui(scr_width, 0);
  draw_notify(scr_width);
  TCOD_console_flush();
}

enum MENU_SCREEN {
  MENU_MAIN,
  MENU_NAME,
};


void draw_menu(enum MENU_SCREEN menu) {
  char *str, *temp;
  int cur_len, max_len, x;
  TCOD_key_t key;
  switch (menu) {
  case MENU_MAIN:
    cprint(1, 1, TCOD_white, TCOD_black, "  New Game");
    cprint(2, 1, TCOD_white, TCOD_black, "  Quit");
    cprint(MENU_CHOICE+1, 1, TCOD_white, TCOD_black, ">");
    TCOD_console_flush();
    break;
  case MENU_NAME:
    cur_len = 0, max_len = MAX_NAME_LEN;
    x = strlen("Enter your name: ") + 1;
    str = calloc(max_len+1, 1);

    do {
      TCOD_console_clear(NULL);
      cprint(1, x, TCOD_white, TCOD_black, "%s_", str);
      cprint(1, x+max_len-1, TCOD_white, TCOD_black, " "); /* erase '_' if at end */
      cprint(1, 1, TCOD_white, TCOD_black, "Enter your name: ");
      cprint(3, 1, TCOD_white, TCOD_black, "Press space for a random name.");
      TCOD_console_flush();

      TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS,&key,NULL,1);
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
    } while (!(key.vk == TCODK_ENTER && cur_len != 0));
    assert (strlen(str) > 0);
    player.name = str;
    break;
  }
}

void handle_menu() {
  TCOD_key_t key;

  draw_menu(MENU_MAIN);
  /* Initial menu screen */
  do {
    TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS,&key,NULL,1);
    switch (key.vk) {
    case TCODK_UP:
      MENU_CHOICE = (MENU_CHOICE == 0) ? MENU_NUM_CHOICES-1 : MENU_CHOICE-1;
      break;
    case TCODK_DOWN:
      MENU_CHOICE = (MENU_CHOICE == MENU_NUM_CHOICES-1) ? 0 : MENU_CHOICE+1;
      break;
    }
    draw_menu(MENU_MAIN);
  } while (key.vk != TCODK_ENTER && key.c != ' ');

  TCOD_console_clear(NULL);

  /* QUIT */
  if (MENU_CHOICE == 1)
    exit(0);

  /* NEW GAME */
  draw_menu(MENU_NAME);

  TCOD_console_clear(NULL);
}
