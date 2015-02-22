#ifndef SYSTEM_H
#define SYSTEM_H

void	game_end();
void	assert_end(bool cond, const char *msg);

void	err_print(const char *msg);
void	message_add(char *str, char *punc);
#endif
