#ifndef DEFS_H
#define DEFS_H

#include <libtcod.h>
#include "priority.h"

/* Define global constants */
extern const char	*GAME_NAME;

#define FPS					60
#define TURN_DELAY			20	/* milliseconds */
#define CON_WIDTH			80
#define CON_HEIGHT			50
#define UI_WIDTH			31

#define MAX_NAME_LEN		UI_WIDTH - 6
#define MESSAGE_LIST_LEN	128

#define FOV_RADIUS			16

#define OFFSET_NOT_X		1	/* x-offset for notification message */
#define OFFSET_NOT_Y		0
#define OFFSET_HP_X			3
#define OFFSET_HP_Y			8
#define OFFSET_MP_X			3
#define OFFSET_MP_Y			9
#define OFFSET_WEP_Y		14
#define OFFSET_WEP_X		3
#define OFFSET_MSG			12

/* Define global variables */
char		*PLAYER_NAME;

extern int	DEPTH;
extern int	MAX_WIDTH;			/* full allocated space for dungeon */
extern int	MAX_HEIGHT;
extern int	CURRENT_WIDTH, CURRENT_HEIGHT;
extern int	DUNGEON_X, DUNGEON_Y;
extern int	VIEW_WIDTH, VIEW_HEIGHT;
extern int	UI_X, UI_Y;

extern int	TURN_COUNT;

extern int	INPUT_MODE;
extern int	CAMERA_X, CAMERA_Y;
extern int	LOOK_X, LOOK_Y;
extern int	INV_POS;			/* scroll position in inventory */

extern int	TURN_DELAY_ON;

/* Define macros */
#define SET_EXPLORED(y, x, n)		(DUNGEON[(y)][(x)].EXPLORED = (n))
#define SET_VISIBLE(y, x, n)		(DUNGEON[(y)][(x)].VISIBLE = (n))
#define SET_TRANSPARENT(y, x, n)	(DUNGEON[(y)][(x)].TRANSPARENT = (n))
#define SET_PASSABLE(y, x, n)		(DUNGEON[(y)][(x)].PASSABLE = (n))
#define CHK_EXPLORED(y, x)			(CHK_IN_BOUNDS((y), (x)) && DUNGEON[(y)][(x)].EXPLORED)
#define CHK_VISIBLE(y, x)			(CHK_IN_BOUNDS((y), (x)) && DUNGEON[(y)][(x)].VISIBLE)
#define CHK_TRANSPARENT(y, x)		(CHK_IN_BOUNDS((y), (x)) && DUNGEON[(y)][(x)].TRANSPARENT)
#define CHK_PASSABLE(y, x)			(CHK_IN_BOUNDS((y), (x)) && DUNGEON[(y)][(x)].PASSABLE)
#define CHK_IN_BOUNDS(y, x)			((y) >= 0 && (x) >= 0 && (y) < MAX_HEIGHT && (x) < MAX_WIDTH)
#define CHK_IN_DUNGEON(y, x)		((y) >= DUNGEON_Y && (y) < DUNGEON_Y + CURRENT_HEIGHT && (x) >= DUNGEON_X && (x) < DUNGEON_X + CURRENT_WIDTH)
#define CHK_IN_VIEW(y, x) \
		((y) >= CAMERA_Y - VIEW_HEIGHT / 2 + 1 && (y) < CAMERA_Y + VIEW_HEIGHT / 2 && (x) >= CAMERA_X - VIEW_WIDTH / 2 && (x) <= CAMERA_X + VIEW_WIDTH / 2)

/* Define enums */
enum INPUT_MODES
{
	INPUT_ACTION,
	INPUT_LOOK,
	INPUT_SCROLL,
	INPUT_INVENTORY,
	INPUT_MESSAGE,
	INPUT_EQUIP
};

typedef enum DIRECTION
{
	DIR_NONE,
	DIR_N,
	DIR_E,
	DIR_S,
	DIR_W,
	DIR_NE,
	DIR_SE,
	DIR_SW,
	DIR_NW
} DIRECTION;

enum GENDER
{
	GEN_MALE,
	GEN_FEMALE
};

enum DUNGEON_TYPE
{
	DUNGEON_MAZE,
	DUNGEON_REGULAR,
	DUNGEON_CAVE
};

enum TILE_TYPE
{
	TILE_NOTHING,
	TILE_FLOOR,
	TILE_WALL,
	TILE_SHAFT,
	TILE_WATER,
	TILE_LAVA,
	TILE_STAIRS_UP,
	TILE_STAIRS_DOWN
};

enum ACTOR_TYPE
{
	ACTOR_PLAYER,
	ACTOR_ENEMY,
	ACTOR_NPC
};

enum ITEM_TYPE
{
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_POTION,
	ITEM_TRINKET,
	ITEM_CONSUMABLE
};

enum FURN_TYPE
{
	FURN_DOOR,
	FURN_BRIDGE
};

/* ---------------------------------------------------------------------------------------------------------------------
    Define structs
 ----------------------------------------------------------------------------------------------------------------------- */
typedef struct ITEM ITEM;
struct ITEM
{
	enum ITEM_TYPE	type;
	char			*name;
	char			*art;
	char			ch;
	TCOD_color_t	col;
};

typedef struct
{
	ITEM	*item;
	int		n;					/* Number of items of the type */
} ITEM_N;

typedef struct
{
	enum ACTOR_TYPE type;
	char			*name;		/* remember to free when removing the actor */
	char			*art;		/* Modifier when displaying name. Can be "a", "an", "the", or "" */
	enum GENDER		gender;
	char			ch;			/* display character */
	TCOD_color_t	col;
	int				x, y;		/* position */
	int				level, exp;
	int				hp_max, hp_cur;
	int				mp_max, mp_cur;
	int				spd;		/* movement priority - lower is faster */

	TCOD_list_t		*inventory; /* remember to free */
	ITEM			*weapon;
	ITEM			*armor;

	char			CAN_MOVE, CAN_WALK, CAN_SWIM, CAN_FLY, CAN_BURROW;

	char			IS_SEEN;	/* whether this actor has been seen by the player */
} ACTOR;

typedef struct
{
	enum FURN_TYPE	type;
	char			*name;
	char			*art;
	char			ch;
	TCOD_color_t	col;
	char			FURN_PASSABLE, FURN_FLAMMABLE;
} FURN;

typedef struct
{
	enum TILE_TYPE	type;
	char			*name;
	char			*art;
	char			ch;
	TCOD_color_t	col_vis;
	TCOD_color_t	col_nonvis;
	ACTOR			*resident;	/* actor currently residing in tile */
	TCOD_list_t		*stash;
	FURN			*furn;
	char			EXPLORED, VISIBLE, TRANSPARENT, PASSABLE;
} DUNGEON_BLOCK;

/* Define global data structures */
DUNGEON_BLOCK	**DUNGEON;
DUNGEON_BLOCK	block_wall;
DUNGEON_BLOCK	block_floor;
TCOD_map_t		fov_map;
TCOD_list_t		actor_type_list;
TCOD_list_t		item_type_list;
TCOD_list_t		block_type_list;
TCOD_list_t		message_list;
TCOD_list_t		message_turn_list;
pri_queue		actor_queue, temp_queue;
#endif /* DEFS_H */
