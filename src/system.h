#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>

void game_end();
void err_print(const char *msg);
void err_fprint(const char *msg);
void assert_end(bool cond, const char *msg);
void error_end(const char *msg);

void message_add(char *str, char *punc);
#endif
