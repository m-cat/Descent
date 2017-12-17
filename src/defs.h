#ifndef DEFS_H
#define DEFS_H

#include "util/priority.h"
#include <libtcod.h>

/* Define a spiffy uint type */
typedef unsigned int uint;

/* Define global constants */
extern const char *GAME_NAME;

/* Game constants */

#define FONT_FILE "terminal.png"

#define FPS 60
#define TURN_DELAY 20 /* milliseconds */
#define CON_WIDTH 80
#define CON_HEIGHT 50
#define UI_WIDTH 31

#define MAX_NAME_LEN UI_WIDTH - 6
#define MESSAGE_LIST_LEN 128

#define FOV_RADIUS 16

/* UI constants */

#define OFFSET_NOT_X 1 /* x-offset for notification message */
#define OFFSET_NOT_Y 0
#define OFFSET_HP_X 3
#define OFFSET_HP_Y 8
#define OFFSET_MP_X 3
#define OFFSET_MP_Y 9
#define OFFSET_WEP_Y 14
#define OFFSET_WEP_X 3
#define OFFSET_MSG 12

/* Define global variables */
char *PLAYER_NAME;

extern uint DEPTH;
/* full allocated space for dungeon */
extern int MAX_WIDTH, MAX_HEIGHT;
extern int CURRENT_WIDTH, CURRENT_HEIGHT;
extern int DUNGEON_X, DUNGEON_Y;
extern int VIEW_WIDTH, VIEW_HEIGHT;
extern int UI_X, UI_Y;

extern long TURN_COUNT;

extern int CAMERA_X, CAMERA_Y;
extern int LOOK_X, LOOK_Y;

extern uint INPUT_MODE;
extern int INV_POS; /* scroll position in inventory */

extern uint TURN_DELAY_ON;
/* speed of scrolling - how many tiles to move at once */
extern uint SCROLL_FACTOR;

/* Define macros */
#define SET_EXPLORED(y, x, n) (DUNGEON[y][x].EXPLORED = (n))
#define SET_VISIBLE(y, x, n) (DUNGEON[y][x].VISIBLE = (n))
#define SET_TRANSPARENT(y, x, n) (DUNGEON[y][x].TRANSPARENT = (n))
#define SET_PASSABLE(y, x, n) (DUNGEON[y][x].PASSABLE = (n))
#define CHK_IN_BOUNDS(y, x)                                                    \
    ((y) > 0 && (x) > 0 && (y) < MAX_HEIGHT && (x) < MAX_WIDTH)
#define CHK_EXPLORED(y, x) (DUNGEON[y][x].EXPLORED)
#define CHK_VISIBLE(y, x) (DUNGEON[y][x].VISIBLE)
#define CHK_TRANSPARENT(y, x) (DUNGEON[y][x].TRANSPARENT)
#define CHK_PASSABLE(y, x) (DUNGEON[y][x].PASSABLE)

/* Define enums */
enum INPUT_MODES {
    INPUT_ACTION,
    INPUT_LOOK,
    INPUT_SCROLL,
    INPUT_INVENTORY,
    INPUT_MESSAGE,
    INPUT_EQUIP
};

typedef enum DIRECTION {
    DIR_NONE,
    DIR_N,
    DIR_E,
    DIR_S,
    DIR_W,
    DIR_NE,
    DIR_SE,
    DIR_SW,
    DIR_NW,
    DIR_END /* mark the end of the enum */
} DIRECTION;

enum GENDER { GEN_MALE, GEN_FEMALE };

enum DUNGEON_TYPE { DUNGEON_MAZE, DUNGEON_REGULAR, DUNGEON_CAVE };

enum TILE_TYPE {
    TILE_NOTHING,
    TILE_FLOOR,
    TILE_WALL,
    TILE_SHAFT,
    TILE_WATER,
    TILE_LAVA,
    TILE_STAIRS_UP,
    TILE_STAIRS_DOWN
};

enum ACTOR_TYPE { ACTOR_PLAYER, ACTOR_ENEMY, ACTOR_NPC };

enum ITEM_TYPE {
    ITEM_JUNK,
    ITEM_WEAPON,
    ITEM_ARMOR,
    ITEM_POTION,
    ITEM_TRINKET,
    ITEM_CONSUMABLE
};

enum OBJECT_TYPE { OBJECT_DOOR, OBJECT_BRIDGE };

/* Define structs */
typedef struct ITEM ITEM;
struct ITEM {
    enum ITEM_TYPE type;
    char *name;
    char *art;
    char ch;
    TCOD_color_t col;
};

typedef struct {
    ITEM *item;
    uint n; /* Number of items of the type */
} ITEM_N;

typedef struct {
    enum ACTOR_TYPE type;
    char *name; /* remember to free when removing the actor */
    char *art;  /* modifier when displaying name -
                 * can be "a", "an", "the", or "" */
    char ch;    /* display character */
    enum GENDER gender;
    TCOD_color_t col;
    int x, y; /* position */
    uint level, exp;
    uint hp_max, hp_cur;
    uint mp_max, mp_cur;
    uint spd; /* movement priority - lower is faster */

    TCOD_list_t *inventory; /* remember to free */
    ITEM *weapon;
    ITEM *armor;

    char CAN_MOVE, CAN_WALK, CAN_SWIM, CAN_FLY, CAN_BURROW;

    /* whether this actor has been seen by the player */
    char IS_SEEN;
} ACTOR;

typedef struct {
    enum OBJECT_TYPE type;
    char *name;
    char *art;
    char ch;
    TCOD_color_t col;
    char OBJECT_PASSABLE, OBJECT_FLAMMABLE;
} OBJECT;

typedef struct {
    enum TILE_TYPE type;
    char *name;
    char *art;
    char ch;
    TCOD_color_t col_vis;
    TCOD_color_t col_nonvis;
    ACTOR *actor; /* actor currently residing in tile */
    TCOD_list_t *stash;
    OBJECT *object;
    char EXPLORED, VISIBLE, TRANSPARENT, PASSABLE;
} DUNGEON_BLOCK;

typedef struct {
    char *msg;
    long turn;
} Msg;

/* Define global data structures */
DUNGEON_BLOCK **DUNGEON;
DUNGEON_BLOCK block_wall;
DUNGEON_BLOCK block_floor;
TCOD_map_t fov_map;

TCOD_list_t MESSAGE_LIST;
pri_queue actor_queue, temp_queue;
#endif /* DEFS_H */
