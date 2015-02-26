#include <stdlib.h>
#include <math.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "actor.h"
#include "player.h"
#include "system.h"
#include "priority.h"
#include "ui.h"
#include "algorithms.h"

#define VIS_SET(x, y)               (SET_VISIBLE(y, x, 1), SET_EXPLORED(y, x, 1))
#define VIS_CLR(x, y)               (SET_VISIBLE(y, x, 0))
#define CALC_DIST(x0, y0, x1, y1)   sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0))
#define FLT_LTE(n1, n2)             ((n1) < (n2) || abs((n2) - (n1)) < .001)

/* Advances a turn for ALL actors apart from player. Actors will move in a
 * consistent order each time and lower speeds mean they move before other actors.
 * If an actor is hasted it will move twice (or more!) in the same turn;
 * if slowed, it will skip a turn.
 */
void advance_turn() {
    ACTOR   *a;
    int     pri;
    /*~~~~~~~~*/

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
    unsigned int    i;
    unsigned int    j;
    ACTOR           *a;
    char            *subject;
    /*~~~~~~~~~~~~~~~~~~~~~*/

    /* Compute FOV */
    TCOD_map_compute_fov(fov_map, (int) player->x, (int) player->y, FOV_RADIUS, 1, FOV_SHADOW);

    /* Set tiles as visible or not */
    for (i = MAX(DUNGEON_Y, CAMERA_Y - CON_HEIGHT + 1); i < MIN(DUNGEON_Y + CURRENT_HEIGHT, CAMERA_Y + CON_HEIGHT); i++) {
        for (j = MAX(DUNGEON_X, CAMERA_X - CON_WIDTH + 1); j < MIN(DUNGEON_X + CURRENT_WIDTH, CAMERA_X + CON_WIDTH); j++) {
            TCOD_map_is_in_fov(fov_map, (int) j, (int) i) ? VIS_SET(j, i) : VIS_CLR(j, i);
            if (CHK_VISIBLE(i, j) && DUNGEON[i][j].resident != NULL) {
                a = DUNGEON[i][j].resident;
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
