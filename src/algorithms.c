#include "algorithms.h"
#include "actor.h"
#include "defs.h"
#include "player.h"
#include "system.h"
#include "ui.h"
#include "util.h"
#include "util/priority.h"
#include <libtcod.h>
#include <math.h>
#include <stdlib.h>

/* Set the tile to be both visible and explored */
void vis_set(int y, int x) {
    SET_VISIBLE(y, x, 1);
    SET_EXPLORED(y, x, 1);
}
/* Clear visibility (but leave explored status) */
void vis_clr(int y, int x) { SET_VISIBLE(y, x, 0); }

/* Advances a turn for ALL actors apart from player. Actors will move in a
 * consistent order each time and lower speeds mean they move before other
 * actors. If an actor is hasted it will move twice (or more!) in the same turn;
 * if slowed, it will skip a turn.
 */
void advance_turn() {
    ACTOR *a;
    int pri;

    while ((a = priq_pop(actor_queue, &pri))) {
        /* Take out actor from the queue, act, put it in a temp queue */
        actor_act(a);
        if (TURN_DELAY_ON && CHK_VISIBLE(a->y, a->x)) {
            TCOD_sys_sleep_milli(TURN_DELAY);
            draw_game();
        }

        priq_push(temp_queue, a, pri);
    }

    /* Put all actors in the temp queue back into the main queue */
    priq_combine(actor_queue, temp_queue);
}

void calc_fov() {
    int i, j;
    ACTOR *a;
    char *subject;

    /* Compute FOV */
    TCOD_map_compute_fov(fov_map, player->x, player->y, FOV_RADIUS, 1,
                         FOV_SHADOW);
    /* Set tiles as visible or not */
    for (i = MAX(DUNGEON_Y, CAMERA_Y - CON_HEIGHT + 1);
         i < MIN(DUNGEON_Y + CURRENT_HEIGHT, CAMERA_Y + CON_HEIGHT); i++) {
        for (j = MAX(DUNGEON_X, CAMERA_X - CON_WIDTH + 1);
             j < MIN(DUNGEON_X + CURRENT_WIDTH, CAMERA_X + CON_WIDTH); j++) {
            TCOD_map_is_in_fov(fov_map, j, i) ? vis_set(i, j) : vis_clr(i, j);

            if (CHK_VISIBLE(i, j) && DUNGEON[i][j].actor != NULL) {
                a = DUNGEON[i][j].actor;

                if (a->IS_SEEN == 0) {
                    /* Seeing an actor for the first time */
                    subject = subject_form(a->art, 1, a->name);
                    message_add(string_create(2, "You see ", subject), ".");
                    free(subject);
                    a->IS_SEEN = 1;
                }
            }
        }
    }
}
