#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "util.h"
#include "player.h"
#include "io.h"

/* Define menu variables */
int MENU_CHOICE = 0; /* persists when user returns to menu */
const int MENU_NUM_CHOICES = 2;

/* Define macros */
#define cprint(y, x, c, args...) (attron(COLOR_PAIR(c)), mvprintw(y, x, args))
#define cprintb(y, x, c, args...) (attron(A_BOLD), cprint(y, x, c, args), attroff(A_BOLD))

/* isalnum() does not work properly with ncurses keys */
int is_alphanum(int c) {
  return ((c >= 'a' && c <= 'z') ||
	  (c >= 'A' && c <= 'Z') ||
	  (c >= '0' && c <= '9'));
}

enum COLOR_PAIR get_actor_color(ACTOR *a) {
  switch (a->type) {
  case ACTOR_PLAYER:
    return C_WHITE_BLACK;
  case ACTOR_ENEMY:
    return C_RED_BLACK;
  case ACTOR_NPC:
    return C_YELLOW_BLACK;
  default:
    return C_GREEN_BLACK;
  }
}

void draw_warnings() {
  if (LINES < CON_HEIGHT || COLS < CON_WIDTH) {
    cprint(0, 0, C_RED_BLACK, "WARNING:");
    cprint(1, 0, C_RED_BLACK, "RESIZE  ");
    cprint(2, 0, C_RED_BLACK, "CONSOLE ");
    cprint(3, 0, C_RED_BLACK, "TO 80x24");
  }
}

void draw_view(int scr_width, int scr_height) {
  int i, j, drawi, drawj;
  DUNGEON_BLOCK *block;
  ACTOR *a;
  for (i = player.y-scr_height/2; i <= player.y+scr_height/2; i++)
    for (j = player.x-scr_width/2; j <= player.x+scr_width/2; j++) {
      drawi = i-player.y+scr_height/2, drawj = j-player.x+scr_width/2;
      /* Check if out of bounds */
      if (i < 0 || i >= CURRENT_HEIGHT || j < 0 || j >= CURRENT_WIDTH) {
	cprint(drawi, drawj, C_WHITE_BLACK, " ");
	continue;
      }
      /* Draw the contents of the block
	 Prioritizes actor > item > tile */
      block = &DUNGEON[i][j];
      a = block->resident;
      if (a != NULL)
	cprintb(drawi, drawj, get_actor_color(a), "%c", a->ch);
      else if (block->type == TILE_FLOOR)
	cprint(drawi, drawj, C_WHITE_BLACK, " ");
      else if (block->type == TILE_WALL)
	cprint(drawi, drawj, C_WHITE_BLACK, "#");
    }
}

void draw_ui(int ui_x, int ui_y) {
  int i, j;
  /* Draw separator and borders */
  for (i = 0; i < CON_HEIGHT+1; i++)
    cprint(i, ui_x, C_BLACK_WHITE, " ");
  if (LINES > CON_HEIGHT)
    for (j = 0; j < CON_WIDTH+1; j++)
      cprint(CON_HEIGHT, j, C_BLACK_WHITE, " ");
  if (COLS > CON_WIDTH)
    for (i = 0; i < CON_HEIGHT+1; i++)
      cprint(i, CON_WIDTH, C_BLACK_WHITE, " ");

  /* Draw name */
  for (j = ui_x+2; j <= CON_WIDTH-2; j++)
    cprintb(ui_y+1, j, C_WHITE_BLACK, "~");
  cprintb(ui_y+1, ui_x+5+(MAX_NAME_LEN-strlen(player.name))/2, C_WHITE_BLACK,
	  " %s ", player.name);

  /* Draw level and exp */
  cprintb(ui_y+3, ui_x+2, C_WHITE_BLACK, "Level: %d", player.level);
  cprintb(ui_y+3, CON_WIDTH-6-intlen(player.exp), C_WHITE_BLACK,
	  "Exp: %d", player.exp);

  /* Draw hp and mp */
  cprintb(ui_y+5, ui_x+2, C_WHITE_BLACK, "HP: %d / %d", 12, 32);
  cprint(ui_y+6, ui_x+3, C_RED_BLACK, "[===================]");
  cprintb(ui_y+7, ui_x+2, C_WHITE_BLACK, "MP: %d / %d", 5, 5);
  cprint(ui_y+8, ui_x+3, C_BLUE_BLACK, "[===================]");

  /* Draw weapon name */
}

void draw_game() {
  int scr_width = min(COLS, CON_WIDTH)-UI_WIDTH;
  int scr_height = min(LINES, CON_HEIGHT);
  draw_view(scr_width, scr_height);
  draw_ui(scr_width, 0);
  draw_warnings();
}

enum MENU_SCREEN {
  MENU_MAIN,
  MENU_NAME,
};

void draw_menu(enum MENU_SCREEN menu) {
  char *str, *temp;
  int cur_len, max_len, key, x;
  switch (menu) {
  case MENU_MAIN:
    cprintb(1, 1, C_WHITE_BLACK, "  New Game");
    cprintb(2, 1, C_WHITE_BLACK, "  Quit");
    cprintb(MENU_CHOICE+1, 1, C_WHITE_BLACK, ">");
    break;
  case MENU_NAME:
    cur_len = 0, max_len = MAX_NAME_LEN;
    x = strlen("Enter your name: ") + 1;
    str = calloc(max_len, 1);

    while (!((key=getch()) == '\n' && cur_len != 0)) {
      clear();
      if ((is_alphanum(key) || key == '-') && cur_len < max_len-1)
	str[cur_len++] = key;
      else if ((key == KEY_BACKSPACE || key == KEY_DC) && cur_len > 0)
	str[--cur_len] = 0;
      else if (key == ' ') {
	temp = name_gen();
	cur_len = strlen(strcpy(str, temp));
	free(temp);
      }
      cprintb(1, x, C_WHITE_BLACK, "%s_", str);
      cprintb(1, x+max_len-1, C_WHITE_BLACK, " "); /* erase '_' if at end */
      cprintb(1, 1, C_WHITE_BLACK, "Enter your name: ");
      cprintb(3, 1, C_WHITE_BLACK, "Press space for a random name.");
      handle_resize();
      msleep(1000/FPS);
    }
    player.name = str;
    break;
  }
}

void handle_menu() {
  int key;

  /* Initial menu screen */
  while ((key=getch()) != '\n') {
    switch (key) {
    case KEY_UP:
      MENU_CHOICE = (MENU_CHOICE == 0) ? MENU_NUM_CHOICES-1 : MENU_CHOICE-1;
      break;
    case KEY_DOWN:
      MENU_CHOICE = (MENU_CHOICE == MENU_NUM_CHOICES-1) ? 0 : MENU_CHOICE+1;
      break;
    }
    handle_resize();
    draw_menu(MENU_MAIN);
    msleep(1000/FPS);
  }
  clear();

  /* QUIT */
  if (MENU_CHOICE == 1)
    exit(0);

  /* NEW GAME */
  draw_menu(MENU_NAME);

  clear();
}