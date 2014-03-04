#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include "defs.h"
#include "util.h"

int kbhit() {
  int ch = getch();

  if (ch != ERR) {
    return ch;
  }
  return 0;
}

int msleep(int ms) {
  struct timespec t1, t2;
  t1.tv_sec = 0;
  t1.tv_nsec = ms*1000000;
  nanosleep(&t1, &t2);

  return 0;
}

/* Returns randum integer in [a, b] */
int rand_int(int a, int b) {
    return rand() % (b - a + 1) + a;
}

double rand_float(double a, double b)
{
    return (double)rand()/RAND_MAX * (b - a) + a;
}

void itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;
     
  /* If %d is specified and D is minus, put `-' in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;
     
  /* Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;
     
      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);
     
  /* Terminate BUF. */
  *p = 0;
     
  /* Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

/* Example usage:
   printf("You see here %s %s!\n", a_or_an(name), name); */
int _a_or_an(const char *s) {
  switch (s[0]) {
  case 'a': case 'e': case 'i': case 'o': case 'u': return 0;
  default: return 1;
  }
}
