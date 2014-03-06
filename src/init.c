#include <ncurses/ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include "defs.h"
#include "init.h"

int handle_signal = 0;
void _handle_resize(int sig) {
  handle_signal = 1;
}
void handle_resize() {
  if (handle_signal) {
    endwin();
    refresh();
    clear();
    resizeterm(LINES, COLS);
    handle_signal = 0;
  }
}

int curses_started = false;
void endCurses() {
  if (curses_started && !isendwin())
    endwin();
}

int init_all() {
  /* Initialize ncurses */
  initscr();
  keypad(stdscr, TRUE);  /* Allow reading arrow keys */
  curs_set(0);           /* Make the cursor invisible */
  noecho();              /* Suppress echoing of user input */
  raw();                 /* Disable line buffering */
  nodelay(stdscr, TRUE); /* Disable delay on getch() */
  atexit(endCurses);
  curses_started = true;

  /* Initialize colors */
  if (!has_colors()) {
    endwin();
    fprintf(stderr, "Your terminal does not support colors.\n");
    exit(1);
  }
  start_color();
  init_pair(C_RED_BLACK, COLOR_RED, COLOR_BLACK);
  init_pair(C_GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
  init_pair(C_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(C_WHITE_RED, COLOR_WHITE, COLOR_RED);
  init_pair(C_WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(C_BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

  /* Initialize misc */
  signal(SIGWINCH, _handle_resize); /* Add signal handler for resize */
  srand(time(NULL));     /* Initialize seed to rand() */

  return 0;
}
