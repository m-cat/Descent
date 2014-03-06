#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include "defs.h"
#include "util.h"

int kbhit() {
  int ch = getch();
  return (ch != ERR) ? ch : 0;
}

int msleep(int ms) {
  struct timespec t1, t2;
  t1.tv_sec = 0;
  t1.tv_nsec = ms*1000000;
  nanosleep(&t1, &t2);

  return 0;
}

/* Returns random integer in [a, b] */
int rand_int(int a, int b) {
    return rand() % (b - a + 1) + a;
}

double rand_float(double a, double b)
{
    return (double)rand()/RAND_MAX * (b - a) + a;
}

/* Example usage:
   printf("You see here %s %s!\n", a_or_an(name), name); */
int _a_or_an(const char *s) {
  switch (s[0]) {
  case 'a': case 'e': case 'i': case 'o': case 'u': return 0;
  default: return 1;
  }
}
