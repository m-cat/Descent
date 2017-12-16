#include "actor.h"
#include "defs.h"
#include "items.h"
#include "models.h"
#include "system.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

ACTOR *actor_create(uint y, uint x, MODEL_ACTOR *model) {
    ACTOR *a = NULL;

    a = malloc(sizeof(ACTOR));
    assert_end(a != NULL, "Could not allocate actor memory.");

    /* Initialize data from model */
    a->type = model->type;
    a->name = model->name; /* we assume these names won't ever change */
    a->art = model->art;
    a->ch = model->ch;
    a->col = model->col;
    a->gender = (rand_float(0, 1) < model->gen_ratio) ? GEN_MALE : GEN_FEMALE;

    a->level = model->level, a->exp = model->exp;
    a->hp_max = rand_unsigned_int(model->hp_min, model->hp_max);
    a->hp_max -= a->hp_max % model->hp_mod;
    a->hp_cur = a->hp_max;
    a->mp_max = rand_unsigned_int(model->mp_min, model->mp_max);
    a->mp_cur = a->mp_max;
    a->spd = model->spd;

    a->inventory = NULL;
    a->weapon = NULL;
    a->armor = NULL;

    a->CAN_MOVE = model->CAN_MOVE;
    a->CAN_WALK = model->CAN_WALK;
    a->CAN_SWIM = model->CAN_SWIM;
    a->CAN_FLY = model->CAN_FLY;
    a->CAN_BURROW = model->CAN_BURROW;

    /* Initialize other data */
    a->y = y;
    a->x = x;
    a->IS_SEEN = 0;
    a->inventory = malloc(sizeof(TCOD_list_t));
    assert_end(a != NULL, "Could not allocate actor inventory memory.");
    *(a->inventory) = TCOD_list_new();

    /* TODO: add possible items to actor's inventory */

    /* Place actor in dungeon and in turn queue */
    DUNGEON[y][x].actor = a;
    if (a->CAN_MOVE) {
        priq_push(actor_queue, a, (int)a->spd);
    }

    return a;
}

void actor_free(ACTOR *a) {
    ITEM_N *iterator;

    /* Delete inventory */
    while (TCOD_list_size(*(a->inventory)) > 0) {
        iterator = TCOD_list_pop(*(a->inventory));
        item_free(iterator->item); /* free the item */
        free(iterator);            /* free the item_n */
    }

    TCOD_list_delete(*(a->inventory));
    free(a->inventory);

    free(a);
}

/* TODO: check if actor is allowed to make the move, check CAN_WALK etc */
int actor_can_move(ACTOR *a, uint y, uint x) {
    if (y == 0 && x == 0) {
        return 1;
    }

    return CHK_PASSABLE(y, x) && DUNGEON[y][x].actor == NULL;
}

void actor_try_move(ACTOR *a, uint y, uint x) {
    /* TODO: add failed movement attempts, i.e. bumping into walls */
    DUNGEON[a->y][a->x].actor = NULL;
    a->y = y;
    a->x = x;
    DUNGEON[a->y][a->x].actor = a;
}

/* Adds item to the actor's inventory */
void actor_add_item(ACTOR *a, ITEM *item) {
    ITEM_N **iterator;
    ITEM_N *item_n = NULL;

    for (iterator = (ITEM_N **)TCOD_list_begin(*(a->inventory));
         iterator != (ITEM_N **)TCOD_list_end(*(a->inventory)); iterator++) {
        /* Item already found in inventory, increment count */
        if (strcmp((*iterator)->item->name, item->name) == 0) {
            (*iterator)->n++;
            item_free(item);
            return;
        }
    }

    /* No stack of the same item exists, create new stack */
    item_n = malloc(sizeof(ITEM_N));
    assert_end(item_n != NULL, "Could not allocate item stack memory.");

    item_n->item = item;
    item_n->n = 1;
    TCOD_list_push(*(a->inventory), (const void *)item_n);
}

/* Removes the i'th item from the actor's inventory, returning it */
ITEM *actor_get_item_i(ACTOR *a, int i) {
    ITEM_N *item_n;
    ITEM *item = NULL;

    if (i == -1) {
        item_n = TCOD_list_peek(*(a->inventory));
    } else {
        item_n = TCOD_list_get(*(a->inventory), i);
    }

    if (item_n->n == 1) {
        item = item_n->item;
        TCOD_list_remove(*(a->inventory), (const void *)item_n);
        free(item_n);
    } else {
        item = malloc(sizeof(ITEM));
        assert_end(item != NULL, "Could not allocate item memory.");
        item_copy(item, item_n->item);
        item_n->n--;
    }

    return item;
}

/* Picks up the i'th item in the stash */
void actor_pickup(ACTOR *a, int i) {
    ITEM *item = item_pickup(a->y, a->x, i);

    /* Add message if visible */
    if (CHK_VISIBLE(a->y, a->x)) {
        message_add(sentence_form(a->art, 1, a->name, "picks up", "pick up",
                                  item->art, 1, item->name),
                    ".");
    }

    actor_add_item(a, item);
}

/* Drops the i'th item in the actor's inventory */
void actor_drop(ACTOR *a, int i) {
    ITEM *item = actor_get_item_i(a, i);

    /* Add message if visible */
    if (CHK_VISIBLE(a->y, a->x)) {
        char *sentence = sentence_form(a->art, 1, a->name, "drops", "drop",
                                       item->art, 1, item->name);
        message_add(sentence, ".");
    }

    item_drop(a->y, a->x, item);
}

/* Wields the i'th item in the actor's inventory */
void actor_wield(ACTOR *a, int i) {
    ITEM *item;

    item = actor_get_item_i(a, i);
    if (a->weapon != NULL) {
        actor_add_item(a, a->weapon);
    }

    a->weapon = item;

    /* Add message if visible */
    if (CHK_VISIBLE(a->y, a->x)) {
        message_add(sentence_form(a->art, 1, a->name, "wields", "wield",
                                  item->art, 1, item->name),
                    ".");
    }
}

/*
 * a attacks b ;
 * TODO: Implement combat calculation
 */
void actor_attack(ACTOR *a, ACTOR *b) {
    message_add(sentence_form(a->art, 1, a->name, "attacks", "attack", b->art,
                              1, b->name),
                "!");
}

void actor_act(ACTOR *a) {
    /* int dx;
     * int dy;
     * do { dir = random_dir;
     * } while (!actor_can_move(a, dy, dx));
     * actor_try_move(a, dy, dx);
     */
}
