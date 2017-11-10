#include "io.h"
#include "defs.h"
#include "items.h"
#include "player.h"
#include "system.h"
#include "util.h"
#include <ctype.h>
#include <libtcod.h>
#include <stdlib.h>
#include <string.h>

/* type = {ALPHA, NUM, ALPHANUMERIC} Returns NULL if user pressed Escape */
char *cinput(uint y, uint x, uint max, INPUT_TYPE type) {
    TCOD_key_t key;
    uint cur_len = 0;
    char *str = calloc(max + 1, 1);

    do {
        cprint(y, x, TCOD_white, TCOD_black, "%s_", str);
        TCOD_console_flush();

        TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL, 1);
        if (((is_alphanum(key.c) && type == ALPHANUM) ||
             (is_alpha(key.c) && type == ALPHA) ||
             (is_num(key.c) && type == NUMERIC)) &&
            cur_len < max - 1) {
            str[cur_len++] = key.c;
            str[cur_len] = 0;
        } else if ((key.vk == TCODK_BACKSPACE || key.vk == TCODK_DELETE) &&
                   cur_len > 0) {
            str[--cur_len] = 0;
        } else if (key.c == ' ') {
            ; /* Implement something here if needed */
        } else if (key.vk == TCODK_ESCAPE) {
            free(str);
            return NULL;
        }
    } while ((key.vk != TCODK_ENTER && key.vk != TCODK_KPENTER) ||
             cur_len == 0);
    assert_end(strlen(str) > 0, "Corrupted string");
    return str;
}
