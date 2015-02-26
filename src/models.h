#ifndef MODELS_H
#define MODELS_H

#include <libtcod.h>
#include "defs.h"

/* ---------------------------------------------------------------------------------------------------------------------
    TODO: have generic models, i.e. one base model for all fruits with character : ;
    Model item struct
 ----------------------------------------------------------------------------------------------------------------------- */
typedef struct
{
    enum ITEM_TYPE  type;
    char            *name;
    char            *art;
    char            ch;
    TCOD_color_t    col;
} MODEL_ITEM;

/* ---------------------------------------------------------------------------------------------------------------------
    Model actor struct
 ----------------------------------------------------------------------------------------------------------------------- */
typedef struct
{
    enum ACTOR_TYPE type;
    char            *name;          /* remember to free when removing the actor */
    char            *art;           /* modifier when displaying name. Can be "a", "an", "the", or "" */
    char            ch;             /* display character */
    float           gen_ratio;      /* ratio of men to women */
    TCOD_color_t    col;

    unsigned int    level, exp;
    unsigned int    hp_min, hp_max; /* range of possible HP values */
    unsigned int    hp_mod;         /* the final HP will be divisible by this number */
    unsigned int    mp_min, mp_max;
    unsigned int    spd;            /* movement priority - lower is faster */

    char            CAN_MOVE, CAN_WALK, CAN_SWIM, CAN_FLY, CAN_BURROW;
} MODEL_ACTOR;

/* ---------------------------------------------------------------------------------------------------------------------
    Model block struct
 ----------------------------------------------------------------------------------------------------------------------- */
typedef struct
{
    enum TILE_TYPE  type;
    char            *name;
    char            *art;
    char            ch;
    TCOD_color_t    col_vis;
    TCOD_color_t    col_nonvis;
    char            EXPLORED, VISIBLE, TRANSPARENT, PASSABLE;
} MODEL_BLOCK;

/* Declare model items */
MODEL_ITEM  model_apple;
MODEL_ITEM  model_diamond;
MODEL_ITEM  model_pickax;

/* Declare model actors */
MODEL_ACTOR model_rat;

/* Declare model blocks */
MODEL_BLOCK model_wall;
MODEL_BLOCK model_floor;
MODEL_BLOCK model_staircase_down;
MODEL_BLOCK model_staircase_up;
MODEL_BLOCK model_water;

/* Function declarations */
void        init_item_models();
void        init_actor_models();
void        init_block_models();

void        init_models();

#endif
