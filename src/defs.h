#ifndef DEFS_H
#define DEFS_H

#include <libtcod.h>

/* Define global constants */
extern const char *GAME_NAME;

#define FPS 60
#define CON_WIDTH 80
#define CON_HEIGHT 50
#define UI_WIDTH 31

#define MAX_NAME_LEN UI_WIDTH-6
#define MESSAGE_LIST_LEN 128

#define FOV_RADIUS 16
#define COL_ACTIVE TCOD_brass
#define COL_INACTIVE TCOD_darker_sepia

/* Define global variables */
extern int DEPTH;
extern int MAX_WIDTH; /* full allocated space for dungeon */
extern int MAX_HEIGHT;
extern int CURRENT_WIDTH, CURRENT_HEIGHT;
extern int DUNGEON_X, DUNGEON_Y;
extern int TURN_COUNT;

extern int INPUT_MODE;
extern int CAMERA_X, CAMERA_Y;
extern int LOOK_X, LOOK_Y;

/* Define macros */
#define IN_BOUNDS(y, x) ((y) >= 0 && (x) >= 0 &&		\
			 (y) < MAX_HEIGHT && (x) < MAX_WIDTH)
#define SET_EXPLORED(y, x, n) (DUNGEON[(y)][(x)].EXPLORED = (n))
#define SET_VISIBLE(y, x, n) (DUNGEON[(y)][(x)].VISIBLE = (n))
#define SET_TRANSPARENT(y, x, n) (DUNGEON[(y)][(x)].TRANSPARENT = (n))
#define SET_PASSABLE(y, x, n) (DUNGEON[(y)][(x)].PASSABLE = (n))
#define CHK_EXPLORED(y, x) (IN_BOUNDS((y),(x)) && DUNGEON[(y)][(x)].EXPLORED)
#define CHK_VISIBLE(y, x) (IN_BOUNDS((y),(x)) && DUNGEON[(y)][(x)].VISIBLE)
#define CHK_TRANSPARENT(y, x) (IN_BOUNDS((y),(x)) && DUNGEON[(y)][(x)].TRANSPARENT)
#define CHK_PASSABLE(y, x) (IN_BOUNDS((y),(x)) && DUNGEON[(y)][(x)].PASSABLE)

/* Define enums */
enum INPUT_MODES {
  INPUT_ACTION,
  INPUT_LOOK,
  INPUT_SCROLL,
  INPUT_INVENTORY,
};

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
  TILE_STAIRS_UP,
  TILE_STAIRS_DOWN,
};

enum ACTOR_TYPE {
  ACTOR_PLAYER,
  ACTOR_ENEMY,
  ACTOR_NPC,
};

enum ITEM_TYPE {
  ITEM_WEAPON     = 0,
  ITEM_ARMOR      = 1,
  ITEM_POTION     = 2,
  ITEM_TRINKET    = 3,
  ITEM_CONSUMABLE = 4,
};

enum FURN_TYPE {
  FURN_DOOR,
  FURN_BRIDGE,
};

/* Define structs */


typedef struct ITEM ITEM;
struct ITEM {
  enum ITEM_TYPE type;
  char *name;
  char ch;
  TCOD_color_t col;
};

typedef struct {
  ITEM *item;
  int n; /* Number of items of the type */
} ITEM_N;

typedef struct {
  enum ACTOR_TYPE type;
  char *name; /* remember to free when removing the actor */
  char ch;    /* display character */
  int x, y;   /* position */
  int level, exp;
  int hp_max, hp_cur;
  int spd; /* movement priority */

  TCOD_list_t *inventory; /* remember to free */

  char CAN_MOVE,
    CAN_WALK,
    CAN_SWIM,
    CAN_FLY,
    CAN_BURROW;
} ACTOR;

typedef struct {
  enum FURN_TYPE type;
  char *name;
  char ch;
  char FURN_PASSABLE,
    FURN_FLAMMABLE;
} FURN;

typedef struct {
  enum TILE_TYPE type;
  char *name;
  char ch;
  ACTOR *resident; /* actor currently residing in tile */
  TCOD_list_t *stash;
  FURN *furn;
  char EXPLORED,
    VISIBLE,
    TRANSPARENT,
    PASSABLE;
} DUNGEON_BLOCK;

/* Define global data structures */
DUNGEON_BLOCK **DUNGEON;
TCOD_map_t fov_map;
TCOD_list_t item_type_list;
TCOD_list_t message_list;

#endif /* DEFS_H */
