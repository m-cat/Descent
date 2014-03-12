#ifndef DRAW_H
#define DRAW_H

/* Define macros */
#define cprint(y, x, c1, c2, args...)			\
  (TCOD_console_set_default_foreground(NULL, c1),	\
   TCOD_console_set_default_background(NULL, c2),	\
   TCOD_console_print(NULL, x, y, args))
#define cprint_center(y, x, c1, c2, args...)		\
  (TCOD_console_set_alignment(NULL, TCOD_CENTER),	\
   cprint(y, x, c1, c2, args),				\
   TCOD_console_set_alignment(NULL, TCOD_LEFT))
#define cprint_right(y, x, c1, c2, args...)		\
  (TCOD_console_set_alignment(NULL, TCOD_RIGHT),	\
   cprint(y, x, c1, c2, args),				\
   TCOD_console_set_alignment(NULL, TCOD_LEFT))

void draw_game();
void draw_menu();

#endif
