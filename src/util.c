#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"

int is_alpha(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_num(int c) {
	return c >= '0' && c <= '9';
}

/* isalnum() does not work properly with ncurses keys */
int is_alphanum(int c) {
	return is_alpha(c) || is_num(c);
}

/* Example usage: printf("You see here %s %s!\n", a_or_an(name), name);
 */
int _a_or_an(const char *s) {
	switch (s[0]) {
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
			return 0;

		default:
			return 1;
	}
}

void capitalize(char *s) {
	s[0] = toupper(s[0]);
}

/* Input: arg count, strings to be concatenated. Returns: pointer to string
 * (should be freed)
 */
char *string_create(int argc, ...) {
	va_list args;
	int		j;
	int		len = 0;
	char	*buf;
	/*~~~~~~~~~~~~*/

	va_start(args, argc);
	for (j = 0; j < argc; j++) {
		len += strlen(va_arg(args, char *));
	}

	va_end(args);

	buf = malloc(len + 1);
	va_start(args, argc);
	for (j = 0; j < argc; j++) {
		(j == 0) ? strcpy(buf, va_arg(args, char *)) : strcat(buf, va_arg(args, char *));
	}

	va_end(args);
	return buf;
}

char *subject_form(char *article, int num, char *subject) {
	char	*art;
	char	*plur;
	char	*ret;
	/*~~~~~~~~~~*/

	if (strcmp(article, "you") == 0) {
		return string_create(1, "you"); /* return a malloc'd string */
	}

	if (num == 1) {
		art = article;
	}
	else {
		art = malloc(16);
		inttostr(art, num, 10);
	}

	plur = (num > 1) ? "s" : "";

	if (strcmp(art, "") == 0) {
		ret = string_create(2, subject, plur);
	}
	else {
		ret = string_create(4, art, " ", subject, plur);
	}

	if (num != 1) {
		free(art);
	}

	return ret;
}

char *sentence_form(char *article1, int num1, char *subject, char *verb_sing, char *verb_plur, char *article2, int num2, char *object) {
	char	*art1;
	char	*art2;
	char	*ret;
	/*~~~~~~~~~~*/

	art1 = subject_form(article1, num1, subject);
	art2 = subject_form(article2, num2, object);

	if (strcmp(article1, "you") == 0 || num1 > 1) {
		ret = string_create(5, art1, " ", verb_plur, " ", art2);
	}
	else {
		ret = string_create(5, art1, " ", verb_sing, " ", art2);
	}

	free(art1);
	free(art2);
	capitalize(ret);
	return ret;
}

/* Got this from stack overflow */
char *str_copy(const char *s) {
	char	*d = malloc(strlen(s) + 1); // Space for length plus nul
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (d == NULL) {
		return NULL;					// No memory
	}

	strcpy(d, s);						// Copy the characters
	return d;							// Return the new string
}

/* Returns random integer in [a, b] */
int rand_int(int a, int b) {
	return TCOD_random_get_int(NULL, MIN(a, b), MAX(a, b));
}

double rand_float(double a, double b) {
	return TCOD_random_get_double(NULL, MIN(a, b), MAX(a, b));
}

int intlen(int a) {
	int i = 1;
	/*~~~~~~*/

	a /= 10;
	while (a) {
		a /= 10, i++;
	}

	return i;
}

/* Converts an integer input to its representation in the provided base. Assumes
 * positive input and base <= 16.
 */
int inttostr(char *buffer, long i, int base) {
	char	*hex_chars = "0123456789abcdef";
	char	result[16];
	char	result2[16] = "";
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (i == 0) {
		strcpy(buffer, "0");
		return 0;
	}

	while (i > 0) {
		strncpy(result, hex_chars + (i % base), 1);
		result[1] = '\0';
		strcat(result, result2);
		strcpy(result2, result);
		i /= base;
	}

	strcpy(buffer, result);
	return 0;
}

/* Builds up a name from pre-defined sequences of letters. Highly configurable in
 * terms of which sequences are possible. Remember to FREE the string after you're
 * done with it.
 */
char *name_gen() {

	/* Define the list of consonant, double consonant, etc. sequences */
	const char		*consonants = "bdfghklmnprsstv";
	const char		*start_dconsonants = "brchcltrthdrslwhphblcrfrwrgrstqu";
	const char		*dconsonants = "brchclcttrthghdrmmttllstqu";
	const char		*end_dconsonants = "chctthghstrm";
	const char		*tconsonants = "strthrchr";
	const char		*vowels = "aaeeiioou";
	const char		*dvowels = "ioiaai";
	char			*word = calloc(MAX_NAME_LEN + 1, 1);
	unsigned int	c;
	unsigned int	m;
	unsigned int	cons_i = strlen(consonants);
	unsigned int	st_dcons_i = strlen(start_dconsonants) / 2;
	unsigned int	dcons_i = strlen(dconsonants) / 2;
	unsigned int	e_dcons_i = strlen(end_dconsonants) / 2;
	unsigned int	tcons_i = strlen(tconsonants) / 3;
	unsigned int	vow_i = strlen(vowels);
	unsigned int	dvow_i = strlen(dvowels) / 2;
	int				vowel = (rand_float(0, 1) < .33) ? 1 : 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Choose initial sequence of letters */
	if (vowel) {
		strncpy(word, vowels + rand_int(0, strlen(vowels) - 1), 1);
	}
	else {

		/* 'w', 'j', and 'z' can only appear at the beginning of a name */
		switch (rand_int(1, 20)) {
			case 1:
				strcpy(word, "w");
				break;

			case 2:
				strcpy(word, "j");
				break;

			case 3:
				strcpy(word, "z");
				break;

			default:
				c = rand_int(0, cons_i + st_dcons_i + tcons_i - 1);
				if (c < cons_i) {
					strncpy(word, consonants + c, 1);
				}
				else if (c < cons_i + st_dcons_i) {
					strncpy(word, start_dconsonants + 2 * (c - cons_i), 2);
				}
				else {
					strncpy(word, tconsonants + 3 * (c - cons_i - st_dcons_i), 3);
				}
		}
	}

	/* Alternate between choosing vowel and consonant sequences */
	m = rand_int(2, 5);
	while (m-- > 0 && strlen(word) < MAX_NAME_LEN - 3) {

		/* using the "goes to" operator --> */
		if ((vowel = !vowel)) {
			c = rand_int(0, vow_i + dvow_i - 1);
			if (c < vow_i) {
				strncat(word, vowels + c, 1);
			}
			else {
				strncat(word, dvowels + 2 * (c - vow_i), 2);
			}
		}
		else {
			if (m <= 0) {
				c = rand_int(0, cons_i + e_dcons_i - 1);
			}
			else {
				c = rand_int(0, cons_i + dcons_i + tcons_i - 1);
			}

			if (c < cons_i) {
				strncat(word, consonants + c, 1);
			}
			else if (c < cons_i + st_dcons_i) {
				if (m <= 0) {
					strncat(word, end_dconsonants + 2 * (c - cons_i), 2);
				}
				else {
					strncat(word, consonants + 2 * (c - cons_i), 2);
				}
			}
			else {
				strncat(word, tconsonants + 3 * (c - cons_i - dcons_i), 3);
			}
		}
	}

	capitalize(word);
	return word;
}
