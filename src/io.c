#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "items.h"
#include "player.h"
#include "io.h"

/* type = {ALPHA, NUM, ALPHANUMERIC} Returns NULL if user pressed Escape */
char* cinput(int y, int x, int max, int type) {
	TCOD_key_t	key;
	int			cur_len = 0;
	char*		str = calloc(max + 1, 1);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	do
	{
		cprint(y, x, TCOD_white, TCOD_black, "%s_", str);
		TCOD_console_flush();

		TCOD_sys_wait_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL, 1);
		if (((is_alphanum(key.c) && type == ALPHANUM) || (is_alpha(key.c) && type == ALPHA) || (is_num(key.c) && type == NUMERIC)
			) && cur_len < max - 1) {
			str[cur_len++] = key.c;
			str[cur_len] = 0;
		}
		else if ((key.vk == TCODK_BACKSPACE || key.vk == TCODK_DELETE) && cur_len > 0) {
			str[--cur_len] = 0;
		}
		else if (key.c == ' ') {
			;			/* Implement something here if needed */
		}
		else if (key.vk == TCODK_ESCAPE) {
			free(str);
			return NULL;
		}
	} while ((key.vk != TCODK_ENTER && key.vk != TCODK_KPENTER) || cur_len == 0);
	assert(strlen(str) > 0);
	return str;
}

void draw_notify() {
	DUNGEON_BLOCK*	block;
	ACTOR*			a;
	FURN*			furn;
	TCOD_list_t*	stash;
	ITEM_N*			item_n = NULL;
	int				j;
	char*			sentence,
					*subject;
	bool			visible;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for (j = 0; j < VIEW_WIDTH; j++) {
		cprint(0, j, TCOD_white, TCOD_black, " ");
	}

	switch (INPUT_MODE) {
		case INPUT_ACTION:
			stash = DUNGEON[player->y][player->x].stash;
			if (stash != NULL) {
				item_n = TCOD_list_peek(*stash);
				if (item_n != NULL) {
					subject = subject_form(item_n->item->art, item_n->n, item_n->item->name);
					sentence = string_create(2, (item_n->n == 1) ? "There's " : "There are ", subject);
					cprint(OFFSET_NOT_Y, OFFSET_NOT_X, TCOD_white, TCOD_black, "%s at your feet. %c%c%c%c%s", sentence,
						   TCOD_COLCTRL_FORE_RGB, 1, 255, 1, (TCOD_list_size(*stash) > 1) ? "[more] " : "");
					free(subject);
					free(sentence);
				}
			}
			break;

		case INPUT_SCROLL:
			cprint(OFFSET_NOT_Y, OFFSET_NOT_X, TCOD_orange, TCOD_black, "[Scrolling]");
			break;

		case INPUT_LOOK:

			/* Tile unexplored OR out of bounds */
			if (!CHK_EXPLORED(LOOK_Y, LOOK_X)) {
				cprint(OFFSET_NOT_Y, OFFSET_NOT_X, TCOD_white, TCOD_black, "You have not explored here.");
				break;
			}

			block = &DUNGEON[LOOK_Y][LOOK_X];
			a = block->resident;
			furn = block->furn;
			if (block->stash != NULL) {
				item_n = TCOD_list_peek(*(block->stash));
			}

			visible = CHK_VISIBLE(LOOK_Y, LOOK_X);

			if (visible && a != NULL) {
				subject = subject_form(a->art, 1, a->name);
			}
			else if (furn != NULL) {
				subject = subject_form(furn->art, 1, furn->name);
			}
			else if (visible && item_n != NULL) {
				subject = subject_form(item_n->item->art, item_n->n, item_n->item->name);
			}
			else {
				subject = subject_form(block->art, 1, block->name);
			}

			/* Tile not visible, display only furn or block */
			if (!visible) {
				cprint(OFFSET_NOT_Y, OFFSET_NOT_X, TCOD_white, TCOD_black, "You remember seeing %s.", subject);
			}
			else {
				cprint(OFFSET_NOT_Y, OFFSET_NOT_X, TCOD_white, TCOD_black, "You see %s.", subject);
			}

			free(subject);
			break;

		default:
			break;
	}
}

void draw_view() {
	int				i,
					j,
					drawi,
					drawj;
	DUNGEON_BLOCK*	block;
	ACTOR*			a;
	FURN*			furn;
	ITEM*			item;
	enum TILE_TYPE	type;
	TCOD_color_t	c2; /* background color */
	bool			is_look;
	/*~~~~~~~~~~~~~~~~~~~~*/

	for (i = CAMERA_Y - VIEW_HEIGHT / 2 + 1; i < CAMERA_Y + VIEW_HEIGHT / 2; i++) {
		for (j = CAMERA_X - VIEW_WIDTH / 2; j <= CAMERA_X + VIEW_WIDTH / 2; j++) {
			drawi = i - CAMERA_Y + VIEW_HEIGHT / 2, drawj= j - CAMERA_X + VIEW_WIDTH / 2;
			is_look = (i == LOOK_Y && j == LOOK_X && INPUT_MODE == INPUT_LOOK);

			/* Check if not explored AND if in bounds of dungeon */
			if (!CHK_EXPLORED(i, j)) {
				if (is_look) {
					cprint(drawi, drawj, TCOD_black, TCOD_amber, " ");
				}
				else {
					cprint(drawi, drawj, TCOD_white, TCOD_black, " ");
				}

				continue;
			}

			block = &DUNGEON[i][j];
			assert(block != NULL);

			/* Check if not visible */
			if (!CHK_VISIBLE(i, j)) {
				if (is_look) {
					cprint(drawi, drawj, TCOD_black, TCOD_amber, "%c", block->ch);
				}
				else {
					cprint(drawi, drawj, block->col_nonvis, TCOD_black, "%c", block->ch);
				}

				continue;
			}

			/* Draw the contents of the block Prioritizes actor > furn > item > tile */
			a = block->resident;
			furn = block->furn;
			type = block->type;
			item = item_get_top(i, j);
			switch (type) {
				case TILE_WATER:	c2 = TCOD_blue; break;
				case TILE_LAVA:		c2 = TCOD_red; break;
				default:			c2 = TCOD_black; break;
			}

			if (a != NULL) {
				cprint(drawi, drawj, a->col, c2, "%c", a->ch);
			}
			else if (furn != NULL) {
				cprint(drawi, drawj, furn->col, c2, "%c", furn->ch);
			}
			else if (item != NULL) {
				cprint(drawi, drawj, item->col, c2, "%c", item->ch);
			}
			else {
				cprint(drawi, drawj, block->col_vis, c2, "%c", block->ch);
			}

			/* Set square being examined to yellow and black */
			if (is_look) {
				TCOD_console_set_char_foreground(NULL, drawj, drawi, TCOD_black);
				TCOD_console_set_char_background(NULL, drawj, drawi, TCOD_amber, TCOD_BKGND_SET);
			}
		}
	}
}

void draw_inventory() {
	ITEM_N **		 iterator;
	int				i;
	TCOD_color_t	c;
	char*			subject;
	/*~~~~~~~~~~~~~~~~~~~~~*/

	cprint_center(UI_Y + 1, UI_X + UI_WIDTH / 2 + 1, TCOD_white, TCOD_black, "Inventory");

	TCOD_console_set_color_control(TCOD_COLCTRL_1, TCOD_sea, TCOD_black);
	cprint_center(UI_Y + 3, UI_X + UI_WIDTH / 2 + 1, TCOD_white, TCOD_black, "e%cx%camine | %cu%cse | %cD%crop", TCOD_COLCTRL_1,
				  TCOD_COLCTRL_STOP, TCOD_COLCTRL_1, TCOD_COLCTRL_STOP, TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
	cprint_center(UI_Y + 4, UI_X + UI_WIDTH / 2 + 1, TCOD_white, TCOD_black, "%cw%cield | %cW%cear", TCOD_COLCTRL_1,
				  TCOD_COLCTRL_STOP, TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);

	if (INV_POS >= TCOD_list_size(*(player->inventory))) {
		INV_POS = MAX(0, TCOD_list_size(*(player->inventory)) - 1);
	}

	for (iterator = (ITEM_N**) TCOD_list_begin(*(player->inventory)), i = 0;
		 iterator != (ITEM_N**) TCOD_list_end(*(player->inventory)); iterator++, i++) {
		c = (i == INV_POS) ? TCOD_azure : TCOD_black;
		subject = subject_form((*iterator)->item->art, (*iterator)->n, (*iterator)->item->name);
		cprint(UI_Y + 6 + i, UI_X + 2, TCOD_white, c, "%c - %s", 'a' + i, subject);
		free(subject);
	}
}

void draw_equip() {
	cprint_center(UI_Y + 1, UI_X + UI_WIDTH / 2 + 1, TCOD_white, TCOD_black, "Equipment");
}

void draw_ui() {
	int		i,
			j;
	char **	 iterator;
	int*	iterator2;
	/*~~~~~~~~~~~~~~*/

	// TCOD_color_t c;
	TCOD_console_set_default_foreground(NULL, TCOD_white);
	TCOD_console_set_default_background(NULL, TCOD_black);

	/* Clear UI area */
	for (i = UI_Y; i < CON_HEIGHT; i++) {
		for (j = UI_X; j < CON_WIDTH; j++) {
			TCOD_console_print(NULL, j, i, " ");
		}
	}

	/* Draw separator and borders */
	for (i = 0; i < CON_HEIGHT; i++) {
		cprint(i, UI_X, TCOD_white, TCOD_black, "|");
	}

	switch (INPUT_MODE) {
		case INPUT_INVENTORY:
			draw_inventory();
			break;

		case INPUT_EQUIP:
			draw_equip();
			break;

		case INPUT_MESSAGE:
			cprint_center(1, UI_X + UI_WIDTH / 2 + 1, TCOD_white, TCOD_black, "Latest Messages");

			for (iterator = (char**) TCOD_list_end(message_list) - 1, i = CON_HEIGHT - 2, iterator2 = (int*) TCOD_list_end(
					 message_turn_list) - 1; i > 2 && iterator != (char**) TCOD_list_begin(message_list) - 1;
				 iterator--, iterator2--, i--) {
				if (*iterator2 >= TURN_COUNT) {
					cprint(i, UI_X + 2, TCOD_white, TCOD_black, "%s", *iterator);
				}
				else {
					cprint(i, UI_X + 2, TCOD_grey, TCOD_black, "%s", *iterator);
				}
			}
			break;

		default:

			/* Draw name */
			for (j = UI_X + 2; j <= CON_WIDTH - 2; j++) {
				cprint(UI_Y + 1, j, TCOD_lime, TCOD_black, "~");
			}

			cprint_center(UI_Y + 1, UI_X + UI_WIDTH / 2 + 1, TCOD_white, TCOD_black, " %s ", player->name);

			/* Draw level and exp */
			cprint(UI_Y + 3, UI_X + 3, TCOD_white, TCOD_black, "Level: %d", player->level);
			cprint(UI_Y + 4, UI_X + 3, TCOD_white, TCOD_black, "Exp: %d / %d", player->exp, player->level * 100);

			/* Draw depth and turn */
			cprint_right(UI_Y + 3, CON_WIDTH - 3, TCOD_white, TCOD_black, "Depth: %d", DEPTH);

			/* Draw hp and mp */
			cprint(UI_Y + OFFSET_HP_Y, UI_X + OFFSET_HP_X, TCOD_white, TCOD_black, "HP: %d / %d", player->hp_cur, player->hp_max);
			cprint(UI_Y + OFFSET_MP_Y, UI_X + OFFSET_MP_X, TCOD_white, TCOD_black, "MP: %d / %d", player->mp_cur, player->mp_max);

			/* Draw weapon name */
			cprint(UI_Y + OFFSET_WEP_Y, UI_X + OFFSET_WEP_X, TCOD_white, TCOD_black, "Weapon: ");
			if (player->weapon == NULL) {
				cprint(UI_Y + OFFSET_WEP_Y, UI_X + OFFSET_WEP_X + strlen("Weapon: "), TCOD_grey, TCOD_black, "none");
			}
			else {
				cprint(UI_Y + OFFSET_WEP_Y, UI_X + OFFSET_WEP_X + strlen("Weapon: "), player->weapon->col, TCOD_black, "%s",
					   player->weapon->name);
			}

			/* Draw messages */
			for (j = UI_X + 1; j < CON_WIDTH; j++) {
				cprint(CON_HEIGHT - OFFSET_MSG, j, TCOD_white, TCOD_black, "~");
			}

			for (iterator = (char**) TCOD_list_end(message_list) - 1, i = CON_HEIGHT - 2, iterator2 = (int*) TCOD_list_end(
					 message_turn_list) - 1; i > CON_HEIGHT - OFFSET_MSG + 1 && iterator != (char**) TCOD_list_begin(message_list) - 1;
				 iterator--, iterator2--, i--) {
				if (*iterator2 >= TURN_COUNT) {
					cprint(i, UI_X + 2, TCOD_white, TCOD_black, "%s", *iterator);
				}
				else {
					cprint(i, UI_X + 2, TCOD_grey, TCOD_black, "%s", *iterator);
				}
			}

			if (i == CON_HEIGHT - OFFSET_MSG + 1 && iterator != (char**) TCOD_list_begin(message_list) - 1 && *iterator2 >= TURN_COUNT) {
				cprint_right(i, CON_WIDTH - 2, TCOD_green, TCOD_black, "[more]");
			}
			break;
	}
}

void draw_game() {
	draw_view();
	draw_ui();
	draw_notify();
	TCOD_console_flush();
}
