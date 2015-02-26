#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

#ifndef MAX
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))
#endif

#define SIGN(n)     ((n) ? ((n > 0) ? 1 : -1) : 0)
#define SWAP(a, b)  ((a) ^= (b), (b) ^= (a), (a) ^= (b))

int is_alpha(int c);
int is_num(int c);
int is_alphanum(int c);

#define a_or_an(s)  (_a_or_an(s) ? "a" : "an")
int             _a_or_an(const char *s);
void            capitalize(char *s);

char            *string_create(unsigned int argc, ...);
char            *subject_form(char *article, unsigned int num, char *subject);
char            *sentence_form(
  char *article1, unsigned int num1, char *subject, char *verb_sing, char *verb_plur, char *article2, unsigned int num2, char *object);

char            *str_copy(const char *s);

int             rand_int(int a, int b);
unsigned int    rand_unsigned_int(unsigned int a, unsigned int b);
double          rand_float(double a, double b);

unsigned int    inttostr(char *buffer, unsigned long i, unsigned int base);
unsigned int    intlen(unsigned int a);

char            *name_gen();

void            MOVE_DIR(unsigned int y, unsigned int x, unsigned int *new_y, unsigned int *new_x, DIRECTION dir, unsigned int amt);
DIRECTION       random_dir();

#endif
