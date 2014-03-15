#ifndef UTIL_H
#define UTIL_H

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define SIGN(n) ((n) ? ((n>0) ? 1 : -1) : 0)
#define SWAP(a,b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

#define a_or_an(s) (_a_or_an(s) ? "a" : "an")
int _a_or_an(const char *s);
void capitalize(char *s);

char* string_create(int argc, ...);
char* subject_form(char *article, int num, char *subject);
char* sentence_form(char *article1, int num1, char *subject, char *verb_sing,
		    char *verb_plur, char *article2, int num2, char *object);
void message_add(char *str, char *punc);

char *strdup (const char *s);

int rand_int(int a, int b);
double rand_float(double a, double b);

int itoa(char *buffer, long i, int base);
int intlen(int a);

char* name_gen();

#endif
