#include "items.h"
#include "defs.h"
#include "io.h"
#include "models.h"
#include "system.h"
#include "util.h"
#include <libtcod.h>
#include <stdlib.h>
#include <string.h>

/* For a deep copy */
void item_copy(ITEM *dest, ITEM *source) {
    *dest = *source;
    dest->name = str_copy(source->name);
    dest->art = str_copy(source->art);
}

ITEM *item_create(MODEL_ITEM *model) {
    ITEM *item = NULL;

    item = malloc(sizeof(ITEM));
    assert_end(item != NULL, "Could not allocate item memory.");

    item->type = model->type;

    /* May need to create duplicates of these strings in the future */
    // TODO: Rework these. Have `name` be a pointer to a (name, art) string
    // tuple in some list
    item->name = model->name;
    item->art = model->art;

    item->ch = model->ch;
    item->col = model->col;

    return item;
}

/* Returns a pointer to the top stack's item. Does NOT remove the item from the
 * stash
 */
ITEM *item_get_top(uint y, uint x) {
    ITEM_N *item_n;

    if (DUNGEON[y][x].stash == NULL ||
        TCOD_list_is_empty(*(DUNGEON[y][x].stash))) {
        return NULL;
    }

    item_n = (ITEM_N *)TCOD_list_peek(*(DUNGEON[y][x].stash));
    return item_n->item;
}

/* Removes and returns an item from position i in the stash */
ITEM *item_pickup(uint y, uint x, int i) {
    ITEM *item;
    ITEM_N *item_n;

    if (i == -1) {
        item_n = (ITEM_N *)TCOD_list_peek(*(DUNGEON[y][x].stash));
    } else {
        item_n = (ITEM_N *)TCOD_list_get(*(DUNGEON[y][x].stash), i);
    }

    item = malloc(sizeof(ITEM));
    item_copy(item, item_n->item);
    if (item_n->n > 1) {
        item_n->n--;
    } else {
        TCOD_list_remove(*(DUNGEON[y][x].stash), (const void *)item_n);
        item_free(item_n->item);
        free(item_n);
    }

    return item;
}

void item_drop(uint y, uint x, ITEM *item) {
    ITEM_N *item_n;
    ITEM_N **iterator;
    TCOD_list_t *stash = DUNGEON[y][x].stash;

    if (stash == NULL) {
        stash = malloc(sizeof(TCOD_list_t));
        *stash = TCOD_list_new();
    } else {
        /* Search for preexisting stack of the same item */
        for (iterator = (ITEM_N **)TCOD_list_begin(*stash);
             iterator != (ITEM_N **)TCOD_list_end(*stash); iterator++) {
            if (strcmp((*iterator)->item->name, item->name) == 0) {
                (*iterator)->n++;
                item_free(item);
                return;
            }
        }
    }

    /* No stack of the same item exists, create new stack */
    item_n = malloc(sizeof(ITEM_N));
    item_n->item = item;
    item_n->n = 1;
    TCOD_list_push(*stash, (const void *)item_n);

    DUNGEON[y][x].stash = stash;
}

void item_place(uint y, uint x, MODEL_ITEM *model) {
    ITEM *item = item_create(model);

    assert_end(item != NULL, "Corrupted item data.");
    item_drop(y, x, item);
}

void item_free(ITEM *item) {
    // TODO: Free name and art when they are loaded from file.

    assert_end(item != NULL, "Corrupted item data.");
    assert_end(item->name != NULL, "Corrupted item data.");

    // free(item->name);
    // free(item->art);
    free(item);
}
