#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"

/* Returns random integer in [a, b] */
int rand_int(int a, int b) {
  return TCOD_random_get_int(NULL, a, b);
}

double rand_float(double a, double b) {
  return TCOD_random_get_double(NULL, a, b);
}

int intlen(int a) {
  int i=1; a /= 10;
  while (a) a /= 10, i++;
  return i;
}

/* Example usage:
   printf("You see here %s %s!\n", a_or_an(name), name); */
int _a_or_an(const char *s) {
  switch (s[0]) {
  case 'a': case 'e': case 'i': case 'o': case 'u': return 0;
  default: return 1;
  }
}

void capitalize(char *s) {
  s[0] = toupper(s[0]);
}

/* Builds up a name from pre-defined sequences of letters.
   Highly configurable in terms of which sequences are possible.
   Remember to FREE the string after you're done with it. */
char* name_gen() {
  /* Define the list of consonant, double consonant, etc. sequences */
  const char *consonants = "bdfghklmnprsstv";
  const char *start_dconsonants = "brchcltrthdrslwhphblcrfrwrgrstqu";
  const char *dconsonants = "brchclcttrthghdrmmttllstqu";
  const char *end_dconsonants = "chctthghstrm";
  const char *tconsonants = "strthrchr";
  const char *vowels = "aeiou";
  const char *dvowels = "ioiaai";
  char *word = calloc(MAX_NAME_LEN+1,1);
  int c, m;
  int cons_i = strlen(consonants),
    st_dcons_i = strlen(start_dconsonants)/2,
    dcons_i = strlen(dconsonants)/2,
    e_dcons_i = strlen(end_dconsonants)/2,
    tcons_i = strlen(tconsonants)/3,
    vow_i = strlen(vowels),
    dvow_i = strlen(dvowels)/2;
  int vowel = (rand_float(0, 1) < .33) ? 1 : 0;

  /* Choose initial sequence of letters */
  if (vowel)
    strncpy(word, vowels+rand_int(0,strlen(vowels)-1), 1);
  else {
    /* 'w', 'j', and 'z' can only appear at the beginning of a name */
    switch (rand_int(1, 20)) {
    case 1:
      strcpy(word, "w"); break;
    case 2:
      strcpy(word, "j"); break;
    case 3:
      strcpy(word, "z"); break;
    default:
      c = rand_int(0, cons_i + st_dcons_i + tcons_i - 1);
      if (c < cons_i)
	strncpy(word, consonants+c, 1);
      else if (c < cons_i + st_dcons_i)
	strncpy(word, start_dconsonants+2*(c-cons_i), 2);
      else
	strncpy(word, tconsonants+3*(c-cons_i-st_dcons_i), 3);
    }
  }

  /* Alternate between choosing vowel and consonant sequences */
  m = rand_int(2,5);
  while (m --> 0 && strlen(word) < MAX_NAME_LEN-3) { /* using the "goes to" operator --> */
    if (vowel = !vowel) {
      c = rand_int(0, vow_i + dvow_i - 1);
      if (c < vow_i)
	strncat(word, vowels+c, 1);
      else
	strncat(word, dvowels+2*(c-vow_i), 2);
    }
    else {
      if (m <= 0)
	c = rand_int(0, cons_i + e_dcons_i - 1);
      else
	c = rand_int(0, cons_i + dcons_i + tcons_i - 1);

      if (c < cons_i)
	strncat(word, consonants+c, 1);
      else if (c < cons_i + st_dcons_i) {
	if (m <= 0)
	  strncat(word, end_dconsonants+2*(c-cons_i), 2);
	else
	  strncat(word, consonants+2*(c-cons_i), 2);
      }
      else
	strncat(word, tconsonants+3*(c-cons_i-dcons_i), 3);
    }
  }

  capitalize(word);
  return word;
}
