#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define SIGN(n) ((n) ? ((n > 0) ? 1 : -1) : 0)
#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

int is_alpha(int c);
int is_num(int c);
int is_alphanum(int c);

#define a_or_an(s) (_a_or_an(s) ? "a" : "an")
int _a_or_an(const char *s);
void capitalize(char *s);

char *string_create(uint argc, ...);
char *subject_form(char *article, uint num, char *subject);
char *sentence_form(char *article1, uint num1, char *subject, char *verb_sing,
                    char *verb_plur, char *article2, uint num2, char *object);

char *str_copy(const char *s);

int rand_int(int a, int b);
uint rand_unsigned_int(uint a, uint b);
double rand_float(double a, double b);

uint inttostr(char *buffer, unsigned long i, uint base);
uint intlen(uint a);

char *name_gen();

void move_dir(int y, int x, int *new_y, int *new_x, DIRECTION dir, uint amt);
DIRECTION
random_dir();

#endif
