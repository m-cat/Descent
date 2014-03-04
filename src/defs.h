#ifndef DEFS_H
#define DEFS_H

#include <ncurses/ncurses.h>

/* Define global constants */
#define FPS 30
#define MAX_WIDTH 560 /* full allocated space for dungeon */
#define MAX_HEIGHT 240

/* Define global variables */
extern int PLAYER_X, PLAYER_Y;
extern int LEVEL;
extern int CURRENT_WIDTH, CURRENT_HEIGHT;

/* Define macros */
#define cprint(y, x, c, args...) (attron(COLOR_PAIR(c)), mvprintw(y, x, args))

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
  TILE_WALL
};

enum ACTOR_TYPE {
  ACTOR_PLAYER,
  ACTOR_ENEMY,
  ACTOR_NPC
};

/* Define structs */
typedef struct ACTOR {
  enum ACTOR_TYPE type;
} actor;

typedef struct DUNGEON_BLOCK {
  enum TILE_TYPE type;
  actor *resident; /* actor currently residing in tile */
} dungeon_block;

/* Define global data structures */
dungeon_block DUNGEON[MAX_HEIGHT][MAX_WIDTH];

#endif /* DEFS_H */
