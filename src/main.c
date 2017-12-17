#include "actor.h"
#include "algorithms.h"
#include "defs.h"
#include "dungeon.h"
#include "init.h"
#include "io.h"
#include "menu.h"
#include "player.h"
#include "system.h"
#include "ui.h"
#include "util.h"
#include <libtcod.h>
#include <stdlib.h>

int main() {
    int next_turn;
    TCOD_key_t key;
    TCOD_event_t ev;
    TCOD_event_t request = TCOD_EVENT_KEY | TCOD_EVENT_MOUSE_PRESS;
    TCOD_mouse_t mouse;

    /* Initialize game */
    init_all(); /* Initialize console and colors */

start:
    /* Main menu */
    if (handle_menu() == 1) {
        exit(1);
    }

    /* Generate dungeon */
    dungeon_gen(DUNGEON_CAVE);
    // dungeon_dump();

    calc_fov();
    draw_game();

    /* MAIN GAME LOOP */
    do {
        do {
            if (TCOD_console_is_window_closed()) {
                game_end();
            }

            ev = TCOD_sys_check_for_event(request, &key, &mouse);
        } while ((ev & request) == 0);

        next_turn =
            handle_input(ev, key.vk, key.c, key.lctrl == 1 || key.rctrl == 1,
                         key.shift == 1, mouse.cx, mouse.cy);
        if (next_turn == 1) {
            /* if player used up a turn */
            TURN_COUNT++;
            advance_turn();
            calc_fov();
        }

        draw_game();
    } while (next_turn >= 0);

    if (next_turn == -1) {
        /* Escape was pressed, return to menu */
        goto start;
    }
}
