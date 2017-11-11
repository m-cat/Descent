#include "dungeon.h"
#include "actor.h"
#include "defs.h"
#include "items.h"
#include "models.h"
#include "player.h"
#include "priority.h"
#include "system.h"
#include "util.h"
#include <libtcod.h>
#include <stdlib.h>
#include <string.h>

/* Use this to make a copy of the block's resources. Usually doing dest = source
 * is fine, though, because we can use the same name and art.
 */
void block_copy(DUNGEON_BLOCK *dest, DUNGEON_BLOCK *source) {
    *dest = *source;
    dest->name = str_copy(source->name);
    dest->art = str_copy(source->art);
}

DUNGEON_BLOCK *block_create(uint y, uint x, MODEL_BLOCK *model) {
    DUNGEON_BLOCK *block = NULL;

    block = malloc(sizeof(DUNGEON_BLOCK));
    assert_end(block != NULL, "Could not allocate block memory.");

    block->type = model->type;
    /* usually don't need to duplicate the name strings */
    block->name = model->name;
    block->art = model->art;
    block->ch = model->ch;
    block->col_vis = model->col_vis;
    block->col_nonvis = model->col_nonvis;

    block->resident = NULL;
    block->stash = NULL;
    block->furn = NULL;

    block->EXPLORED = model->EXPLORED;
    block->VISIBLE = model->VISIBLE;
    block->TRANSPARENT = model->TRANSPARENT;
    block->PASSABLE = model->PASSABLE;

    DUNGEON[y][x] = *block;

    return block;
}

void CLR_OPTS(uint y, uint x) {
    DUNGEON[y][x].PASSABLE = 0;
    DUNGEON[y][x].TRANSPARENT = 0;
    DUNGEON[y][x].VISIBLE = 0;
    DUNGEON[y][x].EXPLORED = 0;
}

void CLR_BLOCK(uint y, uint x) {
    ITEM_N *iterator;
    DUNGEON_BLOCK *block = &DUNGEON[y][x];

    assert_end(block != NULL, "Corrupted dungeon data.");
    CLR_OPTS(y, x);

    block->name = NULL;
    block->art = NULL;
    /* Don't free the resident here; we do it in dungeon_clear(). */
    block->resident = NULL;
    if (block->furn != NULL) {
        free(block->furn);
        block->furn = NULL;
    }

    /* Delete every item in a stash */
    if (block->stash != NULL) {
        while (TCOD_list_size(*(block->stash)) > 0) {
            iterator = TCOD_list_pop(*(block->stash));
            item_delete(iterator->item); /* free the item */
            free(iterator);              /* free the item_n */
        }

        TCOD_list_delete(*(block->stash));
        free(block->stash);
    }
}

/* Clear all data from every dungeon block. Set the dungeon to be all walls. */
void dungeon_clear() {
    uint i;
    uint j;
    int pri;
    ACTOR *a;

    /* Clear all blocks */
    for (i = 0; i < MAX_HEIGHT; i++) {
        for (j = 0; j < MAX_WIDTH; j++) {
            CLR_BLOCK(i, j);
            block_create(i, j, &model_wall);
        }
    }

    /* Delete all actors */
    while ((a = priq_pop(actor_queue, &pri))) {
        /* Take out actor from the queue and delete it */
        actor_delete(a);
    }

    /* Delete the fov map */
    TCOD_map_delete(fov_map);
}

/* Resizes a dungeon by amount resize_x, resize_y. If shift_left or shift_up are
 * true, the dungeon will be resized in the negative direction. That is, all
 * elements will be shifted over to simulate a negatively-expanded grid.
 */
void dungeon_resize(uint resize_y, uint resize_x, uint shift_up,
                    uint shift_left) {
    uint i;
    uint j;
    uint prev_height = MAX_HEIGHT;
    uint prev_width = MAX_WIDTH;
    DUNGEON_BLOCK *test_ptr;

    if (shift_up) {
        DUNGEON_Y += resize_y;
    }

    if (shift_left) {
        DUNGEON_X += resize_x;
    }

    if (resize_y) {
        MAX_HEIGHT += resize_y;
        test_ptr = realloc(DUNGEON, MAX_HEIGHT * sizeof(DUNGEON_BLOCK *));
        if (test_ptr == NULL) {
            free(DUNGEON);
            error_end("Could not allocate memory");
        }
        *DUNGEON = test_ptr;

        /* If resize_y < 0, shift over all the rows */
        if (shift_up) {
            for (i = prev_height; i > 0; i--) {
                DUNGEON[i - 1 + resize_y] = DUNGEON[i - 1];
            }

            for (i = resize_y; i > 0; i--) {
                DUNGEON[i - 1] =
                    calloc((size_t)MAX_WIDTH, sizeof(DUNGEON_BLOCK));
                for (j = 0; j < MAX_WIDTH; j++) {
                    block_create(i - 1, j, &model_wall);
                }
            }
        } else {
            for (i = prev_height; i < MAX_HEIGHT; i++) {
                DUNGEON[i] = calloc(MAX_WIDTH, sizeof(DUNGEON_BLOCK));
                for (j = 0; j < MAX_WIDTH; j++) {
                    block_create(i, j, &model_wall);
                }
            }
        }
    }

    if (resize_x) {
        MAX_WIDTH += resize_x;
        for (i = 0; i < MAX_HEIGHT; i++) {
            DUNGEON[i] = realloc(DUNGEON[i], MAX_WIDTH * sizeof(DUNGEON_BLOCK));
            if (shift_left) {
                /* Shift over all the columns */
                for (j = prev_width; j > 0; j--) {
                    DUNGEON[i][j - 1 + resize_x] = DUNGEON[i][j - 1];
                }

                for (j = resize_x; j > 0; j--) {
                    /* Since this dungeon block has the old values, we need to
                     * prevent CLR_BLOCK() from calling free() on an existing
                     * furn. */
                    DUNGEON[i][j].furn = NULL;
                    DUNGEON[i][j].stash = NULL; /* Same deal here. */
                    CLR_BLOCK(i, j - 1);
                    block_create(i, j - 1, &model_wall);
                }
            } else {
                for (j = prev_width; j < MAX_WIDTH; j++) {
                    /* This dungeon block will have garbage values; we need to
                     * prevent CLR_BLOCK() from calling free() on them. */
                    DUNGEON[i][j].furn = NULL;
                    DUNGEON[i][j].stash = NULL;
                    CLR_BLOCK(i, j);
                    block_create(i, j, &model_wall);
                }
            }
        }
    }
}

void dungeon_next() {
    DEPTH++;
    dungeon_gen(DUNGEON_CAVE);
    message_add(string_create(1, "You descend down the stairs"), "!");
}

void dungeon_set_fov() {
    uint i;
    uint j;

    fov_map = TCOD_map_new((int)MAX_WIDTH, (int)MAX_HEIGHT);
    for (i = DUNGEON_Y; i < DUNGEON_Y + CURRENT_HEIGHT; i++) {
        for (j = DUNGEON_X; j < DUNGEON_X + CURRENT_WIDTH; j++) {
            TCOD_map_set_properties(fov_map, (int)j, (int)i,
                                    CHK_TRANSPARENT(i, j), CHK_PASSABLE(i, j));
        }
    }
}

DIRECTION random_valid_path(uint x, uint y, uint maze_x, uint maze_y,
                            uint width, uint height) {
    DIRECTION paths[4];
    uint count = 0;

    if (y > maze_y + 1 && DUNGEON[y - 2][x].type == TILE_WALL) {
        paths[count++] = DIR_N;
    }

    if (x < maze_x + width - 3 && DUNGEON[y][x + 2].type == TILE_WALL) {
        paths[count++] = DIR_E;
    }

    if (y < maze_y + height - 3 && DUNGEON[y + 2][x].type == TILE_WALL) {
        paths[count++] = DIR_S;
    }

    if (x > maze_x + 1 && DUNGEON[y][x - 2].type == TILE_WALL) {
        paths[count++] = DIR_W;
    }

    return count ? paths[rand_unsigned_int(0, count - 1)] : DIR_NONE;
}

/* Generates a maze within the specified boundaries. width and height should be
 * odd numbers.
 */
void dungeon_gen_maze(uint x, uint y, uint maze_x, uint maze_y, uint width,
                      uint height) {
    enum DIRECTION d;

    assert_end(width % 2 == 1, "Assertion failed: width % 2 == 1");
    assert_end(height % 2 == 1, "Assertion failed: height % 2 == 1");

    block_create(y, x, &model_floor);
    while ((d = random_valid_path(x, y, maze_x, maze_y, width, height)) !=
           DIR_NONE) {
        switch (d) {
        case DIR_N:
            block_create(y - 1, x, &model_floor);
            dungeon_gen_maze(x, y - 2, maze_x, maze_y, width, height);
            break;

        case DIR_E:
            block_create(y, x + 1, &model_floor);
            dungeon_gen_maze(x + 2, y, maze_x, maze_y, width, height);
            break;

        case DIR_S:
            block_create(y + 1, x, &model_floor);
            dungeon_gen_maze(x, y + 2, maze_x, maze_y, width, height);
            break;

        case DIR_W:
            block_create(y, x - 1, &model_floor);
            dungeon_gen_maze(x - 2, y, maze_x, maze_y, width, height);
            break;

        default:
            break;
        }
    }
}

void dungeon_gen_cave(uint goal) {
    uint i = 1;
    uint x = MAX_WIDTH / 2;
    uint y = MAX_HEIGHT / 2;
    uint resize_x;
    uint resize_y;
    uint shift_left;
    uint shift_up;

    CURRENT_HEIGHT = CURRENT_WIDTH = 3;
    DUNGEON_X = x - 1, DUNGEON_Y = y - 1;
    block_create(y, x, &model_staircase_down);

    while (i < goal) {
        switch (rand_unsigned_int(DIR_N, DIR_W)) {
        case DIR_N:
            y -= 1;
            break;

        case DIR_E:
            x += 1;
            break;

        case DIR_S:
            y += 1;
            break;

        case DIR_W:
            x -= 1;
            break;

        default:
            break;
        }

        if (DUNGEON[y][x].type == TILE_WALL) {
            i++;
            resize_x = resize_y = 0;

            if (y <= DUNGEON_Y) {
                DUNGEON_Y -= 1;
                CURRENT_HEIGHT += 1;
            } else if (y >= DUNGEON_Y + CURRENT_HEIGHT - 1) {
                CURRENT_HEIGHT += 1;
            }

            if (x <= DUNGEON_X) {
                DUNGEON_X -= 1;
                CURRENT_WIDTH += 1;
            } else if (x >= DUNGEON_X + CURRENT_WIDTH - 1) {
                CURRENT_WIDTH += 1;
            }

            /* When we hit an edge, we shift over the whole dungeon by 128 and
             * update all the variables accordingly
             */
            shift_left = (DUNGEON_X <= 1);
            shift_up = (DUNGEON_Y <= 1);
            if ((DUNGEON_X + CURRENT_WIDTH >= MAX_WIDTH - 1) || shift_left) {
                resize_x = 128;
            }

            if ((DUNGEON_Y + CURRENT_HEIGHT >= MAX_HEIGHT - 1) || shift_up) {
                resize_y = 128;
            }

            if (resize_x || resize_y) {
                /* expand dungeon size in memory */
                dungeon_resize(resize_y, resize_x, shift_up, shift_left);
                if (shift_up) {
                    y += resize_y;
                }

                if (shift_left) {
                    x += resize_x;
                }
            }

            block_create(y, x, &model_floor);
        }
    }

    block_create(y, x, &model_staircase_up);
    player_place(y, x);
}

typedef struct {
    uint x1, x2;
    uint y1, y2;
    enum DIRECTION dir;
} DUNGEON_WALL;

void dungeon_gen(enum DUNGEON_TYPE type) {
    dungeon_clear();

    switch (type) {
    case DUNGEON_MAZE:
        DUNGEON_X = DUNGEON_Y = 2;
        CURRENT_WIDTH = CURRENT_HEIGHT = 11;
        dungeon_gen_maze(3, 3, DUNGEON_X, DUNGEON_Y, CURRENT_WIDTH,
                         CURRENT_HEIGHT);
        player_place(3, 3);
        break;

    case DUNGEON_CAVE:
        dungeon_gen_cave(1000);
        break;

    case DUNGEON_REGULAR:
        // dungeon_gen_reg();
        break;
    }

    dungeon_place_items();
    dungeon_place_enemies();
    dungeon_set_fov();
}

/* Place items */
void dungeon_place_items() {
    uint i = 12;
    uint x;
    uint y;

    while (i-- > 0) {
        do {
            x = rand_unsigned_int(DUNGEON_X + 1, DUNGEON_X + CURRENT_WIDTH - 1);
            y = rand_unsigned_int(DUNGEON_Y + 1,
                                  DUNGEON_Y + CURRENT_HEIGHT - 1);
        } while (DUNGEON[y][x].type != TILE_FLOOR);
        item_place(y, x,
                   rand_unsigned_int(0, 1) ? &model_diamond : &model_apple);
    }

    item_place(y, x, &model_pickax);
}

/* Place enemies */
void dungeon_place_enemies() {
    uint i = 50;
    uint x;
    uint y;

    while (i-- > 0) {
        do {
            x = rand_unsigned_int(DUNGEON_X + 1, DUNGEON_X + CURRENT_WIDTH - 1);
            y = rand_unsigned_int(DUNGEON_Y + 1,
                                  DUNGEON_Y + CURRENT_HEIGHT - 1);
        } while (DUNGEON[y][x].type != TILE_FLOOR);
        actor_create(y, x, rand_unsigned_int(0, 1) ? &model_rat : &model_rat);
    }
}
