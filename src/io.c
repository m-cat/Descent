#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "items.h"
#include "player.h"
#include "io.h"


TCOD_color_t get_actor_color(ACTOR *a) {
  switch (a->type) {
  case ACTOR_PLAYER:
    return TCOD_white;
  case ACTOR_ENEMY:
    return TCOD_red;
  case ACTOR_NPC:
    return TCOD_lime;
  default:
    return TCOD_white;
  }
}

#define OFFSET_NOT 1 /* x-offset for notification message */
void draw_notify(int scr_width) {
  DUNGEON_BLOCK *block;
  ACTOR *a;
  FURN *furn;
  TCOD_list_t *stash;
  ITEM_N *item_n = NULL;
  enum TILE_TYPE type;
  int j;
  char *sentence, *subject;

  for (j = 0; j < scr_width; j++)
    cprint(0, j, TCOD_white, TCOD_black, " ");

  switch (INPUT_MODE) {
  case INPUT_ACTION:
    stash = DUNGEON[player.y][player.x].stash;
    if (stash != NULL) {
      item_n = TCOD_list_peek(*stash);
      if (item_n != NULL) {
	subject = subject_form(item_n->item->art, item_n->n,
			       item_n->item->name);
	sentence = string_create(2, (item_n->n == 1) ?
				 "There's " : "There are ", subject);
	cprint(0, OFFSET_NOT, TCOD_white, TCOD_black, "%s at your feet. %s",
	       sentence, (TCOD_list_size(*stash) > 1) ? "(more) " : "");
	free(subject);
	free(sentence);
      }
    }
    break;
  case INPUT_SCROLL:
    cprint(0, OFFSET_NOT, TCOD_white, TCOD_black, "Scrolling...");
    break;
  case INPUT_LOOK:
    block = &DUNGEON[LOOK_Y][LOOK_X];
    a = block->resident;
    furn = block->furn;
    type = block->type;
    if (block->stash != NULL)
      item_n = TCOD_list_peek(*(block->stash));
    if (a != NULL) {
      subject = subject_form(a->art, 1, a->name);
      cprint(0, OFFSET_NOT, TCOD_white, TCOD_black, "You see %s.", subject);
      free(subject);
    }
    else if (furn != NULL) {
      subject = subject_form(furn->art, 1, furn->name);
      cprint(0, OFFSET_NOT, TCOD_white, TCOD_black, "You see %s.", subject);
      free(subject);
    }
    else if (item_n != NULL) {
      subject = subject_form(item_n->item->art, item_n->n, item_n->item->name);
      cprint(0, OFFSET_NOT, TCOD_white, TCOD_black, "You see %s.", subject);
      free(subject);
    }
    else {
      subject = subject_form(block->art, 1, block->name);
      cprint(0, OFFSET_NOT, TCOD_white, TCOD_black, "You see %s.", subject);
      free(subject);
    }
    break;
  default:
    break;
  }
}

void draw_view(int scr_width, int scr_height) {
  int i, j, drawi, drawj;
  DUNGEON_BLOCK *block;
  ACTOR *a;
  FURN *furn;
  ITEM *item;
  enum TILE_TYPE type;
  TCOD_color_t c2; /* background color */

  for (i = CAMERA_Y-scr_height/2+1; i < CAMERA_Y+scr_height/2; i++)
    for (j = CAMERA_X-scr_width/2; j <= CAMERA_X+scr_width/2; j++) {
      drawi = i-CAMERA_Y+scr_height/2, drawj = j-CAMERA_X+scr_width/2;
      /* Check if out of bounds */
      if (i < DUNGEON_Y || i >= DUNGEON_Y+CURRENT_HEIGHT ||
	  j < DUNGEON_X || j >= DUNGEON_X+CURRENT_WIDTH) {
	cprint(drawi, drawj, TCOD_white, TCOD_black, " ");
	continue;
      }
      /* Check if not explored */
      else if (!CHK_EXPLORED(i, j)) {
	cprint(drawi, drawj, TCOD_white, TCOD_black, " ");
	continue;
      }

      block = &DUNGEON[i][j];
      /* Check if not visible */
      if (!CHK_VISIBLE(i, j)) {
	cprint(drawi, drawj, block->col_nonvis, TCOD_black, "%c", block->ch);
	continue;
      }
      /* Draw the contents of the block
	 Prioritizes actor > furn > item > tile */
      a = block->resident;
      furn = block->furn;
      type = block->type;
      item = item_get_top(i, j);
      switch (type) {
      case TILE_WATER: c2 = TCOD_blue; break;
      case TILE_LAVA: c2 = TCOD_red; break;
      default: c2 = TCOD_black; break;
      }

      if (a != NULL)
	cprint(drawi, drawj, get_actor_color(a), c2, "%c", a->ch);
      else if (furn != NULL)
	cprint(drawi, drawj, furn->col, c2, "%c", furn->ch);
      else if (item != NULL)
	cprint(drawi, drawj, item->col, c2, "%c", item->ch);
      else
	cprint(drawi, drawj, block->col_vis, c2, "%c", block->ch);

      /* Set square being examined to yellow and black */
      if (INPUT_MODE == INPUT_LOOK && i == LOOK_Y && j == LOOK_X) {
	TCOD_console_set_char_foreground(NULL, drawj, drawi, TCOD_black);
	TCOD_console_set_char_background(NULL, drawj, drawi, TCOD_amber,
					 TCOD_BKGND_SET);
      }
    }
}

void draw_inventory(int ui_x, int ui_y) {
  ITEM_N **iterator;
  int i;

  cprint_center(ui_y+1, ui_x+UI_WIDTH/2+1, TCOD_white, TCOD_black,
		"Inventory");

  cprint_center(ui_y+3, ui_x+UI_WIDTH/2+1, TCOD_white, TCOD_black,
		"Examine | Use | Drop");
  cprint_center(ui_y+4, ui_x+UI_WIDTH/2+1, TCOD_white, TCOD_black,
		"Wield | Wear");

  for (iterator = (ITEM_N**)TCOD_list_begin(*(player.inventory)), i = 0;
       iterator != (ITEM_N**)TCOD_list_end(*(player.inventory)); iterator++, i++) {
    if ((*iterator)->n > 1)
      cprint(ui_y+6+i, ui_x+2, TCOD_white, TCOD_black, "%c - %s x%d",
	     'a'+i, (*iterator)->item->name, (*iterator)->n);
    else
      cprint(ui_y+6+i, ui_x+2, TCOD_white, TCOD_black, "%c - %s",
	     'a'+i, (*iterator)->item->name);
  }
}

void draw_ui(int ui_x, int ui_y) {
  int i, j;
  char **iterator;

  TCOD_console_set_default_foreground(NULL, TCOD_white);
  TCOD_console_set_default_background(NULL, TCOD_black);
  /* Clear UI area */
  for (i = ui_y; i < CON_HEIGHT; i++)
    for (j = ui_x; j < CON_WIDTH; j++)
      TCOD_console_print(NULL, j, i, " ");

  /* Draw separator and borders */
  for (i = 0; i < CON_HEIGHT; i++)
    cprint(i, ui_x, TCOD_black, TCOD_white, " ");

  switch (INPUT_MODE) {
  case INPUT_INVENTORY:
    draw_inventory(ui_x, ui_y);
    break;
  default:
     /* Draw name */
    for (j = ui_x+2; j <= CON_WIDTH-2; j++)
      cprint(ui_y+1, j, TCOD_lime, TCOD_black, "~");
    cprint_center(ui_y+1, ui_x+UI_WIDTH/2+1,
		  TCOD_white, TCOD_black, " %s ", player.name);

    /* Draw level and exp */
    cprint(ui_y+3, ui_x+3, TCOD_white, TCOD_black, "Level: %d", player.level);
    cprint(ui_y+4, ui_x+3, TCOD_white, TCOD_black,
	   "Exp: %d / %d", player.exp, player.level*100);

    /* Draw depth and turn*/
    cprint_right(ui_y+3, CON_WIDTH-3, TCOD_white, TCOD_black,
		 "Depth: %d", DEPTH);
    cprint_right(ui_y+4, CON_WIDTH-3, TCOD_white, TCOD_black,
		 "Turn: %d", TURN_COUNT+1);

    /* Draw hp and mp */
#define OFFSET_HP 7
    cprint(ui_y+OFFSET_HP, ui_x+10, TCOD_white, TCOD_black, "%d / %d", 12, 32);
    cprint(ui_y+OFFSET_HP+1, ui_x+3, TCOD_white, TCOD_black, "HP: [");
    for (j = ui_x+8; j < CON_WIDTH-3; j++)
      cprint(ui_y+OFFSET_HP+1, j, TCOD_flame, TCOD_black, "=");
    cprint(ui_y+OFFSET_HP+1, CON_WIDTH-3, TCOD_white, TCOD_black, "]");

    cprint(ui_y+OFFSET_HP+3, ui_x+10, TCOD_white, TCOD_black, "%d / %d", 5, 5);
    cprint(ui_y+OFFSET_HP+4, ui_x+3, TCOD_white, TCOD_black, "MP: [");
    for (j = ui_x+8; j < CON_WIDTH-3; j++)
      cprint(ui_y+OFFSET_HP+4, j, TCOD_azure, TCOD_black, "=");
    cprint(ui_y+OFFSET_HP+4, CON_WIDTH-3, TCOD_white, TCOD_black, "]");

    /* Draw weapon name */

    /* Draw messages */
#define OFFSET_MSG 12
    for (j = ui_x+1; j < CON_WIDTH; j++)
      cprint(CON_HEIGHT-OFFSET_MSG, j, TCOD_white, TCOD_black, "~");
    for (iterator = (char**)TCOD_list_end(message_list)-1, i = CON_HEIGHT-2;
	 i > CON_HEIGHT-OFFSET_MSG+1 && iterator != (char**)TCOD_list_begin(message_list)-1;
	 iterator--, i--) {
      cprint(i, ui_x+2, TCOD_white, TCOD_black, "%s", *iterator);
    }
    break;
  }
}

void draw_game() {
  int scr_width = CON_WIDTH-UI_WIDTH;
  int scr_height = CON_HEIGHT;

  draw_view(scr_width, scr_height);
  draw_ui(scr_width, 0);
  draw_notify(scr_width);
  TCOD_console_flush();
}





