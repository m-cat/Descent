#ifndef UTIL_H
#define UTIL_H

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef SIGN
#define SIGN(n) ((n) ? ((n)/abs(n)) : 0)
#endif

ssize_t getline2(char *line, size_t *n, FILE *stream);

int kbhit();
int msleep(int ms);
int rand_int(int a, int b);
double rand_float(double a, double b);
int intlen(int a);

#define a_or_an(s) (_a_or_an(s) ? "a" : "an")
int _a_or_an(const char *s);
void capitalize(char *s);
char* name_gen();

#endif
