#ifndef DEFS_H
#define DEFS_H

#include <ncurses/ncurses.h>

/* Define global constants */
#define FPS 30
#define MAX_WIDTH 560 /* full allocated space for dungeon */
#define MAX_HEIGHT 240
#define CON_WIDTH 80
#define CON_HEIGHT 24
#define UI_WIDTH 24

/* Define global variables */
extern int LEVEL;
extern int CURRENT_WIDTH, CURRENT_HEIGHT;

/* Define macros */
#define cprint(y, x, c, args...) (attron(COLOR_PAIR(c)), mvprintw(y, x, args))
#define cbprint(y, x, c, args...) (attron(A_BOLD), cprint(y, x, c, args), attroff(A_BOLD))

/* Define enums */
enum COLOR_PAIR {
  C_NOTHING, /* Don't use this - colors pairs must be >0 */
  C_RED_BLACK,
  C_GREEN_BLACK,
  C_YELLOW_BLACK,
  C_BLUE_BLACK,
  C_MAGENTA_BLACK,
  C_CYAN_BLACK,
  C_WHITE_BLACK
};

enum DIRECTION {
  DIR_NONE,
  DIR_N,
  DIR_NE,
  DIR_E,
  DIR_SE,
  DIR_S,
  DIR_SW,
  DIR_W,
  DIR_NW
};

enum DUNGEON_TYPE {
  DUNGEON_MAZE,
  DUNGEON_REGULAR,
  DUNGEON_CAVE
};

enum TILE_TYPE {
  TILE_NOTHING,
  TILE_FLOOR,
  TILE_WALL,
  TILE_SHAFT,
};

enum ACTOR_TYPE {
  ACTOR_PLAYER,
  ACTOR_ENEMY,
  ACTOR_NPC
};

enum ITEM_TYPE {
  ITEM_WEAPON,
  ITEM_ARMOR,
  ITEM_POTION
};

enum OPT_TRAVEL {
  CAN_MOVE   = 0x01,
  CAN_WALK   = 0x02,
  CAN_SWIM   = 0x04,
  CAN_FLY    = 0x08,
  CAN_BURROW = 0x10,
};

/* Define structs */
typedef struct ACTOR {
  enum ACTOR_TYPE type;
  char *name;
  char ch; /* display character */
  int x, y;
  int level, exp;
  int hp_max, hp_cur;
  /* Define a bunch more variables... */
  enum OPT_TRAVEL opt_travel;
} actor;

typedef struct ITEM {
  enum ITEM_TYPE type;
} item;

typedef struct ITEM_STACK {
  item *held_item;
  struct ITEM_STACK *next;
} item_stack;

typedef struct DUNGEON_BLOCK {
  enum TILE_TYPE type;
  actor *resident; /* actor currently residing in tile */
  item_stack *items;
} dungeon_block;

/* Define global data structures */
dungeon_block DUNGEON[MAX_HEIGHT][MAX_WIDTH];

#endif /* DEFS_H */
