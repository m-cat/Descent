#include <libtcod.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "priority.h"
#include "util.h"
#include "system.h"
#include "dungeon.h"
#include "player.h"
#include "system.h"
#include "models.h"
#include "init.h"

/* Do all initializations for the game */
int init_all() {
    unsigned int    i;
    /*~~~~~~~~~~~~~~*/

    /* Font must be set before initializing root console */
    TCOD_console_set_custom_font("courier12x12_aa_tc.png", TCOD_FONT_LAYOUT_TCOD, 0, 0);

    /* Initialize TCOD console */
    TCOD_console_init_root(CON_WIDTH, CON_HEIGHT, GAME_NAME, 0, TCOD_RENDERER_SDL);

    /* Initialize models */
    init_models();

    /* Initialize data structures */
    DUNGEON = malloc((size_t) MAX_HEIGHT * sizeof(DUNGEON_BLOCK *));
    for (i = 0; i < MAX_HEIGHT; i++) {
        DUNGEON[i] = calloc((size_t) MAX_WIDTH, sizeof(DUNGEON_BLOCK));
        assert_end(DUNGEON[i] != NULL, "Could not allocate memory.");
    }

    fov_map = TCOD_map_new((int) MAX_WIDTH, (int) MAX_HEIGHT);
    message_list = TCOD_list_allocate(MESSAGE_LIST_LEN);
    message_turn_list = TCOD_list_allocate(MESSAGE_LIST_LEN);
    message_add(string_create(2, "Welcome to ", GAME_NAME), "!");
    actor_queue = priq_new(sizeof(ACTOR *));
    temp_queue = priq_new(sizeof(ACTOR *));

    /* Initialize misc */
    TCOD_sys_set_fps(FPS);
    TCOD_console_set_background_flag(NULL, TCOD_BKGND_SET);
    TCOD_console_set_keyboard_repeat(0, 0);

    return 0;
}
