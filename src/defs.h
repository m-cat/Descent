#ifndef DEFS_H
#define DEFS_H

#include <ncurses/ncurses.h>

/* Define global constants */
#define FPS 30
#define MAX_WIDTH 560 /* full allocated space for dungeon */
#define MAX_HEIGHT 240
#define CON_WIDTH 80
#define CON_HEIGHT 24
#define UI_WIDTH 25

#define MAX_NAME_LEN UI_WIDTH-11

/* Define global variables */
extern int LEVEL;
extern int CURRENT_WIDTH, CURRENT_HEIGHT;

/* Define macros */
#define C(c1, c2) ((10*c2) + (c1))
#define Cr(c1, c2, r) ((r) ? ((10*c1) + (c2)) : ((10*c2) + (c1)))

/* Define enums */
enum DIRECTION {
  DIR_NONE,
  DIR_N,
  DIR_E,
  DIR_S,
  DIR_W,
  DIR_NE,
  DIR_SE,
  DIR_SW,
  DIR_NW,
};

enum DUNGEON_TYPE {
  DUNGEON_MAZE,
  DUNGEON_REGULAR,
  DUNGEON_CAVE,
};

enum TILE_TYPE {
  TILE_NOTHING,
  TILE_FLOOR,
  TILE_WALL,
  TILE_SHAFT,
  TILE_WATER,
  TILE_LAVA,
};

enum ACTOR_TYPE {
  ACTOR_PLAYER,
  ACTOR_ENEMY,
  ACTOR_NPC
};

enum ITEM_TYPE {
  ITEM_WEAPON,
  ITEM_ARMOR,
  ITEM_POTION,
  ITEM_TRINKET,
};

enum FURN_TYPE {
  FURN_DOOR,
  FURN_BRIDGE,
};

enum OPT_TRAVEL {
  CAN_MOVE   = 0x01,
  CAN_WALK   = 0x02,
  CAN_SWIM   = 0x04,
  CAN_FLY    = 0x08,
  CAN_BURROW = 0x10,
};

enum OPT_FURN {
  FURN_PASSABLE  = 0x01,
  FURN_FLAMMABLE = 0x02,
};

/* Define structs */
typedef struct {
  enum ACTOR_TYPE type;
  char *name; /* remember to free when removing the actor */
  char ch; /* display character */
  int x, y; /* position */
  int level, exp;
  int hp_max, hp_cur;
  int spd; /* movement priority */
  /* Define a bunch more variables... */
  int opt_travel;
} ACTOR;

typedef struct ITEM ITEM;
struct ITEM {
  enum ITEM_TYPE type;
  char *name;
  char ch;
};

typedef struct ITEM_STACK ITEM_STACK;
struct ITEM_STACK {
  ITEM *item;
  ITEM_STACK *next;
};

typedef struct {
  enum FURN_TYPE type;
  char *name;
  char ch;
  int opt_furn;
} FURN;

typedef struct {
  enum TILE_TYPE type;
  ACTOR *resident; /* actor currently residing in tile */
  ITEM_STACK *items;
  FURN *furn;
} DUNGEON_BLOCK;

/* Define global data structures */
DUNGEON_BLOCK DUNGEON[MAX_HEIGHT][MAX_WIDTH];

#endif /* DEFS_H */
