#include "system.h"
#include "defs.h"
#include "io.h"
#include "util.h"
#include <libtcod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Expand this function to do any needed cleanup in the future TODO: SAVING */
void game_end() { exit(0); }

/* Prints an error to the screen */
void err_print(const char *msg) {
    cprint(0, 0, TCOD_black, TCOD_red, "ERROR: %s", msg);
    cprint(1, 0, TCOD_black, TCOD_red, "Will try to save data.");
    cprint(2, 0, TCOD_black, TCOD_red, "Press any key to quit.");
}

/* Prints an error to a file */
void err_fprint(const char *msg) {
    FILE *err_file;

    err_file = fopen("error_log.txt", "w");
    fprintf(err_file, "ERROR: %s", msg);
    fclose(err_file);
}

/* If cond is false, prints msg and quits */
void assert_end(bool cond, const char *msg) {
    if (!cond) {
        err_print(msg);
        err_fprint(msg);
        TCOD_console_wait_for_keypress(true);
        game_end();
    }
}

/* Display an error and terminate game */
void error_end(const char *msg) {
    err_print(msg);
    err_fprint(msg);
    TCOD_console_wait_for_keypress(true);
    game_end();
}

/* Adds a string to the message list. `str` must have been malloc'd. */
void message_add(char *str, char *punc) {
    char *first;
    char *add; /* additional lines */
    uint cutoff;
    int max_len = UI_WIDTH - 3;
    Msg *msg = malloc(sizeof(Msg));

    while (strlen(str) >= (size_t) max_len) {
        cutoff = max_len;
        while (str[cutoff] != ' ' && cutoff > 0) {
            cutoff--;
        }

        add = malloc(cutoff + 1);
        strncpy(add, str, cutoff);
        add[cutoff] = 0;
        msg->msg = add;
        msg->turn = TURN_COUNT;
        TCOD_list_push(MESSAGE_LIST, (const void *) msg);
        str += cutoff;
        str[0] = ' ';
    }

    msg->msg = string_create(2, str, punc);
    msg->turn = TURN_COUNT;
    TCOD_list_push(MESSAGE_LIST, (const void *) msg);
    free(str);

    /* Remove earlier messages */
    while (TCOD_list_size(MESSAGE_LIST) > MESSAGE_LIST_LEN) {
        first = TCOD_list_get(MESSAGE_LIST, 0);
        TCOD_list_remove(MESSAGE_LIST, first);
        free(first);
    }
}
