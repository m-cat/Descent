#include <stdio.h>
#include <stdlib.h>
#include <libtcod.h>
#include "util.h"
#include "io.h"
#include "defs.h"
#include "system.h"

/* Expand this function to do any needed cleanup in the future TODO: SAVING */
void game_end() {
    exit(0);
}

/* Prints an error to the screen */
void err_print(const char *msg) {
    cprint(0, 0, TCOD_black, TCOD_red, "ERROR: %s", msg);
    cprint(1, 0, TCOD_black, TCOD_red, "Will try to save data.");
    cprint(2, 0, TCOD_black, TCOD_red, "Press any key to quit.");
}

/* Prints an error to a file */
void err_fprint(const char *msg) {
    FILE    *err_file;
    /*~~~~~~~~~~~~~~*/

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

/* Adds a string to the message list. str must have been malloc'd. */
void message_add(char *str, char *punc) {
    char            *first = str;
    char            *add;   /* additional lines */
    unsigned int    cutoff;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~*/

    #define MAX_LEN (UI_WIDTH - 3)
    while (strlen(str) >= MAX_LEN) {
        cutoff = MAX_LEN;
        while (str[cutoff] != ' ') {
            cutoff--;
        }

        add = malloc(cutoff + 1);
        strncpy(add, str, cutoff);
        add[cutoff] = 0;
        TCOD_list_push(message_list, (const void *) add);
        TCOD_list_push(message_turn_list, (const void *) TURN_COUNT);
        str += cutoff;
        str[0] = ' ';
    }

    TCOD_list_push(message_list, (const void *) string_create(2, str, punc));
    TCOD_list_push(message_turn_list, (const void *) TURN_COUNT);
    free(first);

    while (TCOD_list_size(message_list) > MESSAGE_LIST_LEN) {
        first = TCOD_list_get(message_list, 0);
        TCOD_list_remove(message_list, first);
        free(first);
        TCOD_list_remove(message_turn_list, TCOD_list_get(message_turn_list, 0));
    }
}
