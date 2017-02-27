/*
 * io.h
 *
 * Purpose:
 *
 *  Provide user-friendly input/output functions.
 *  cprint is a wrapper to TCOD_console_print which handles colors and supports
 *   a variable amount of arguments.
 *
 * Examples:
 *
 *
 */

#ifndef IO_H
#define IO_H

typedef enum INPUT_TYPE
{
    NUMERIC,
    ALPHA,
    ALPHANUM
} INPUT_TYPE;

/* Define macros */
#define cprint(y, x, c1, c2, ...) \
        (TCOD_console_set_default_foreground(NULL, c1), \
         TCOD_console_set_default_background(NULL, c2), \
         TCOD_console_print(NULL, (int) (x), (int) (y), __VA_ARGS__))
#define cprint_center(y, x, c1, c2, ...) \
        (TCOD_console_set_alignment(NULL, TCOD_CENTER), \
         cprint((int) (y), (int) (x), c1, c2, __VA_ARGS__), \
         TCOD_console_set_alignment(NULL, TCOD_LEFT))
#define cprint_right(y, x, c1, c2, ...) \
        (TCOD_console_set_alignment(NULL, TCOD_RIGHT), \
         cprint((int) (y), (int) (x), c1, c2, __VA_ARGS__), \
         TCOD_console_set_alignment(NULL, TCOD_LEFT))

char    *cinput(unsigned int y, unsigned int x, unsigned int max, INPUT_TYPE type);

#endif
