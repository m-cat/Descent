#ifndef UTIL_H
#define UTIL_H

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

int kbhit();
int msleep(int ms);
int rand_int(int a, int b);
double rand_float(double a, double b);
void itoa (char *buf, int base, int d);

#define a_or_an(s) (_a_or_an(s) ? "a" : "an")
int _a_or_an(const char *s);

#endif
