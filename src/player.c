#include "player.h"
#include "actor.h"
#include "defs.h"
#include "dungeon.h"
#include "items.h"
#include "system.h"
#include "util.h"
#include <libtcod.h>
#include <stdlib.h>

ACTOR *player = NULL;

bool chk_in_view(int y, int x) {
    return y >= CAMERA_Y - VIEW_HEIGHT / 2 + 1 &&
           y < CAMERA_Y + VIEW_HEIGHT / 2 && x >= CAMERA_X - VIEW_WIDTH / 2 &&
           x <= CAMERA_X + VIEW_WIDTH / 2;
}

void player_create() {
    player = malloc(sizeof(ACTOR));
    assert_end(player != NULL, "Could not allocate player memory.");

    /* Initialize data (from save file?) */
    player->type = ACTOR_PLAYER;
    player->name = PLAYER_NAME;
    player->art = "you";
    player->ch = '@';
    player->col = TCOD_white;
    player->gender = GEN_MALE; /* TODO: add player genders */

    player->level = 1, player->exp = 0;
    player->hp_max = 8;
    player->hp_cur = 8;
    player->mp_max = 2;
    player->mp_cur = 2;
    player->spd = 1;

    player->inventory = NULL;
    player->weapon = NULL;
    player->armor = NULL;

    player->CAN_MOVE = 1;
    player->CAN_WALK = 1;
    player->CAN_SWIM = 1;
    player->CAN_FLY = 0;
    player->CAN_BURROW = 0;

    player->IS_SEEN = 1;

    /* Create inventory */
    player->inventory = malloc(sizeof(TCOD_list_t));
    assert_end(player != NULL, "Could not allocate actor inventory memory.");
    *(player->inventory) = TCOD_list_new();
}

void player_place(int y, int x) {
    /* Initialize the player */
    if (player == NULL) {
        player_create();
    }

    player->y = y, player->x = x;
    DUNGEON[y][x].actor = player;
    CAMERA_X = player->x, CAMERA_Y = player->y;
    LOOK_X = player->x, LOOK_Y = player->y;
}

void handle_scroll(DIRECTION dir) {
    move_dir(CAMERA_Y, CAMERA_X, &CAMERA_Y, &CAMERA_X, dir, SCROLL_FACTOR);
}

void handle_look(DIRECTION dir) {
    int new_y;
    int new_x;

    move_dir(LOOK_Y, LOOK_X, &new_y, &new_x, dir, 1);

    if (chk_in_view(new_y, new_x)) {
        LOOK_Y = new_y;
        LOOK_X = new_x;
    }
}

void handle_inv(DIRECTION dir) {
    int inv_size;

    move_dir(INV_POS, 0, &INV_POS, NULL, dir, 1);
    inv_size = TCOD_list_size(*(player->inventory));

    if (INV_POS == inv_size) {
        /* INV_POS went past inventory size */
        INV_POS = 0;
    } else if (INV_POS > inv_size) {
        /* INV_POS went below zero */
        INV_POS = inv_size - 1;
    }
}

int try_move(DIRECTION dir) {
    ACTOR *a;
    int new_y;
    int new_x;

    move_dir(player->y, player->x, &new_y, &new_x, dir, 1);

    if ((a = DUNGEON[new_y][new_x].actor) != NULL) {
        actor_attack(player, a);
        return 1;
    } else if (actor_can_move(player, new_y, new_x)) {
        actor_try_move(player, new_y, new_x);
        return 1;
    }

    return 0;
}

/* Possible modes: - Action, Examine, Scroll, Inventory, Message, Equip Action
 * Mode: Tries to perform a player action corresponding to the pressed key.
 * Returns 1 if an action was peformed
 */
int handle_direction(DIRECTION dir) {
    switch (INPUT_MODE) {
    case INPUT_ACTION:
        return try_move(dir);

    case INPUT_SCROLL:
        handle_scroll(dir);
        return 0;

    case INPUT_LOOK:
        handle_look(dir);
        return 0;

    case INPUT_INVENTORY:
        handle_inv(dir);
        return 0;

    default:
        return 0;
    }
}

int handle_input(TCOD_event_t ev, TCOD_keycode_t key, char ch, int ctrl,
                 int shift, int m_x, int m_y) {
    int ret = 0;
    uint i;

    /* Change input modes */
    if ((INPUT_MODE != INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS &&
        key == TCODK_ESCAPE) {
        INPUT_MODE = INPUT_ACTION; /* Escape -> default screen */
    } else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS &&
               key == TCODK_ESCAPE) {
        return -1; /* Escape -> Main Menu */
    } else if ((INPUT_MODE == INPUT_ACTION) && ctrl) {
        INPUT_MODE = INPUT_SCROLL;
    } else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'x') {
        INPUT_MODE = INPUT_LOOK;
    } else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'i') {
        INPUT_MODE = INPUT_INVENTORY;
    } else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'm') {
        INPUT_MODE = INPUT_MESSAGE;
    } else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'e') {
        INPUT_MODE = INPUT_EQUIP;
    } else if ((INPUT_MODE == INPUT_ACTION || INPUT_MODE == INPUT_LOOK) &&
               ev == TCOD_EVENT_MOUSE_PRESS) {
        m_y += player->y - CON_HEIGHT / 2;
        m_x += player->x - (CON_WIDTH - UI_WIDTH) / 2;
        if (chk_in_view(m_y, m_x)) {
            if (INPUT_MODE == INPUT_LOOK && LOOK_Y == m_y && LOOK_X == m_x) {
                INPUT_MODE = INPUT_ACTION;
            } else {
                INPUT_MODE = INPUT_LOOK;
            }

            LOOK_Y = m_y;
            LOOK_X = m_x;
        }
    } else if ((INPUT_MODE == INPUT_SCROLL) && !ctrl) {
        INPUT_MODE = INPUT_ACTION;
    } else if ((INPUT_MODE == INPUT_LOOK) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'x') {
        INPUT_MODE = INPUT_ACTION;
    } else if ((INPUT_MODE == INPUT_INVENTORY) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'i') {
        INPUT_MODE = INPUT_ACTION;
    } else if ((INPUT_MODE == INPUT_MESSAGE) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'm') {
        INPUT_MODE = INPUT_ACTION;
    } else if ((INPUT_MODE == INPUT_EQUIP) && ev == TCOD_EVENT_KEY_PRESS &&
               ch == 'e') {
        INPUT_MODE = INPUT_ACTION;
    }

    /* Check key release */
    if (ev == TCOD_EVENT_KEY_RELEASE) {
        if (key == TCODK_CONTROL) {
            CAMERA_Y = player->y;
            CAMERA_X = player->x;
        }

        return 0;
    }

    /* Temporarily increment TURN_COUNT so that any messages that are generated
     * are displayed properly.
     */
    TURN_COUNT++;

    /* Key was pressed */
    switch (key) {
    case TCODK_KP8:
    case TCODK_UP:
        ret = handle_direction(DIR_N);
        break;

    case TCODK_RIGHT:
    case TCODK_KP6:
        ret = handle_direction(DIR_E);
        break;

    case TCODK_KP2:
    case TCODK_DOWN:
        ret = handle_direction(DIR_S);
        break;

    case TCODK_KP4:
    case TCODK_LEFT:
        ret = handle_direction(DIR_W);
        break;

    case TCODK_KP9:
        ret = handle_direction(DIR_NE);
        break;

    case TCODK_KP3:
        ret = handle_direction(DIR_SE);
        break;

    case TCODK_KP1:
        ret = handle_direction(DIR_SW);
        break;

    case TCODK_KP7:
        ret = handle_direction(DIR_NW);
        break;

    case TCODK_KP5: /* Take up turn doing nothing */
        ret = (INPUT_MODE == INPUT_ACTION);
        break;

    case TCODK_PRINTSCREEN:
    case TCODK_F12:
        TURN_COUNT--;
        TCOD_sys_save_screenshot(NULL);
        message_add(
            string_create(
                1, "A screenshot has been saved into the working directory"),
            ".");
        TURN_COUNT++;
        break;

    case TCODK_CHAR:
        if (INPUT_MODE == INPUT_INVENTORY) {
            i = (uint)(ch - 'a');
            if ((int) i < TCOD_list_size(*(player->inventory))) {
                INV_POS = i;
            }
        }

        switch (ch) {
        /* pickup item */
        // case 'g':
        case ',':
            if (shift) {
                /* Go upstairs */
                if (INPUT_MODE == INPUT_ACTION) {
                    if (DUNGEON[player->y][player->x].type == TILE_STAIRS_UP) {
                        /* Take the stairs up */
                        dungeon_next();
                        ret = 1;
                    } else {
                        message_add("You can't go up here", ".");
                    }
                }
            } else if (INPUT_MODE == INPUT_ACTION) {
                /* Pick up item */
                if (item_get_top(player->y, player->x) != NULL) {
                    actor_pickup(player, -1);
                    ret = 1;
                }
            }
            break;

        case 'd':
            if (shift) {
                /* Drop item */
                if (INPUT_MODE == INPUT_ACTION) {
                    if (TCOD_list_size(*(player->inventory)) > 0) {
                        actor_drop(player, -1);
                        ret = 1;
                    }
                } else if (INPUT_MODE == INPUT_INVENTORY) {
                    if (TCOD_list_size(*(player->inventory)) > (int) INV_POS) {
                        actor_drop(player, (int) INV_POS);
                        ret = 1;
                    }
                }
                break;
            } else {
                if (INPUT_MODE == INPUT_INVENTORY) {
                    /* don't drop with 'd' in inventory */
                    break;
                }
            }

        case 'w': /* wield item */
            if (INPUT_MODE == INPUT_INVENTORY) {
                if (TCOD_list_size(*(player->inventory)) > (int) INV_POS) {
                    actor_wield(player, (int) INV_POS);
                    ret = 1;
                }
            }
            break;

        case '.':
            if (shift) {
                /* Go downstairs */
                if (INPUT_MODE == INPUT_ACTION) {
                    if (DUNGEON[player->y][player->x].type ==
                        TILE_STAIRS_DOWN) {
                        /* Take the stairs down */
                        dungeon_next();
                        ret = 1;
                    } else {
                        message_add("You can't go down here", ".");
                    }
                }
            }
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    /* Update camera to center on player */
    if (INPUT_MODE == INPUT_ACTION) {
        CAMERA_X = player->x;
        CAMERA_Y = player->y;
    }

    /* If move was made, look coordinates should be reset */
    if (ret) {
        LOOK_X = player->x;
        LOOK_Y = player->y;
    }

    TURN_COUNT--;
    return ret;
}
