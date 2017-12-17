#include "models.h"
#include "defs.h"
#include <libtcod.h>

/* Define model items, then actors, then areas */
const MODEL_ITEM DEFAULT_ITEM = {
    .type = ITEM_JUNK, .name = "", .art = "", .ch = '?', .col = {0, 0, 0}};

const MODEL_ACTOR DEFAULT_ACTOR = {.type = ACTOR_ENEMY,
                                   .name = "",
                                   .art = "",
                                   .ch = '?',
                                   .gen_ratio = 0.5,
                                   .col = {0, 0, 0},

                                   .level = 0,
                                   .exp = 0,
                                   .hp_min = 0,
                                   .hp_max = 0,
                                   .hp_mod = 1,
                                   .mp_min = 0,
                                   .mp_max = 0,
                                   .spd = 1,

                                   .CAN_MOVE = 1,
                                   .CAN_WALK = 1,
                                   .CAN_SWIM = 1,
                                   .CAN_FLY = 0,
                                   .CAN_BURROW = 0};

const MODEL_BLOCK DEFAULT_BLOCK = {.type = TILE_NOTHING,
                                   .name = "",
                                   .art = "",
                                   .ch = '?',
                                   .col_vis = {0, 0, 0},
                                   .col_nonvis = {0, 0, 0},

                                   .EXPLORED = 0,
                                   .VISIBLE = 0,
                                   .TRANSPARENT = 0,
                                   .PASSABLE = 0};

void init_item_models() {
    /* Initialize apple model */
    model_apple = DEFAULT_ITEM; /* set defaults */
    model_apple.type = ITEM_CONSUMABLE;
    model_apple.name = "apple";
    model_apple.art = "an";
    model_apple.ch = ':';
    model_apple.col = (TCOD_color_t){255, 63, 0};

    /* Initialize diamond model */
    model_diamond = DEFAULT_ITEM; /* set defaults */
    model_diamond.type = ITEM_TRINKET;
    model_diamond.name = "diamond";
    model_diamond.art = "a";
    model_diamond.ch = ',';
    model_diamond.col = (TCOD_color_t){0, 191, 255};

    /* Initialize pickax model */
    model_pickax = DEFAULT_ITEM; /* set defaults */
    model_pickax.type = ITEM_WEAPON;
    model_pickax.name = "pickax";
    model_pickax.art = "a";
    model_pickax.ch = ']';
    model_pickax.col = (TCOD_color_t){196, 136, 124};
}

void init_actor_models() {
    /* Initialize rat model */
    model_rat = DEFAULT_ACTOR; /* set defaults */
    model_rat.type = ACTOR_ENEMY;
    model_rat.name = "dungeon rat";
    model_rat.art = "a";
    model_rat.ch = 'r';
    model_rat.col = (TCOD_color_t){196, 136, 124};
}

void init_block_models() {
    /* Initialize wall model */
    model_wall = DEFAULT_BLOCK; /* set defaults */
    model_wall.type = TILE_WALL;
    model_wall.name = "wall";
    model_wall.art = "a";
    model_wall.ch = '#';
    model_wall.col_vis = (TCOD_color_t){158, 134, 100};
    model_wall.col_nonvis = (TCOD_color_t){63, 50, 31};

    /* Initialize floor model */
    model_floor = DEFAULT_BLOCK; /* set defaults */
    model_floor.type = TILE_FLOOR;
    model_floor.name = "floor";
    model_floor.art = "the";
    model_floor.ch = '.';
    model_floor.col_vis = (TCOD_color_t){158, 134, 100};
    model_floor.col_nonvis = (TCOD_color_t){63, 50, 31};
    model_floor.TRANSPARENT = 1;
    model_floor.PASSABLE = 1;

    /* Initialize downward staircase model */
    model_staircase_down = DEFAULT_BLOCK; /* set defaults */
    model_staircase_down.type = TILE_STAIRS_DOWN;
    model_staircase_down.name = "downward staircase";
    model_staircase_down.art = "a";
    model_staircase_down.ch = '>';
    model_staircase_down.col_vis = TCOD_white;
    model_staircase_down.col_nonvis = TCOD_white;
    model_staircase_down.TRANSPARENT = 1;
    model_staircase_down.PASSABLE = 1;

    /* Initialize upward staircase model */
    model_staircase_up = DEFAULT_BLOCK; /* set defaults */
    model_staircase_up.type = TILE_STAIRS_UP;
    model_staircase_up.name = "upward staircase";
    model_staircase_up.art = "a";
    model_staircase_up.ch = '<';
    model_staircase_up.col_vis = TCOD_white;
    model_staircase_up.col_nonvis = TCOD_white;
    model_staircase_up.TRANSPARENT = 1;
    model_staircase_up.PASSABLE = 1;

    /* Initialize water model */
    model_water = DEFAULT_BLOCK; /* set defaults */
    model_water.type = TILE_WATER;
    model_water.name = "water";
    model_water.art = "";
    model_water.ch = '~';
    model_water.col_vis = TCOD_blue;
    model_water.col_nonvis = TCOD_blue;
    model_water.TRANSPARENT = 1;
}

void init_models() {
    init_item_models();
    init_actor_models();
    init_block_models();
}
