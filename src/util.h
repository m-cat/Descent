#ifndef UTIL_H
#define UTIL_H

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define SIGN(n) ((n) ? ((n>0) ? 1 : -1) : 0)
#define SWAP(a,b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

int rand_int(int a, int b);
double rand_float(double a, double b);
int intlen(int a);

#define a_or_an(s) (_a_or_an(s) ? "a" : "an")
int _a_or_an(const char *s);
void capitalize(char *s);
char* name_gen();

#endif
