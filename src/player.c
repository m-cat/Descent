#include <stdlib.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "items.h"
#include "actor.h"
#include "dungeon.h"
#include "player.h"

ACTOR *player = NULL;

void player_place(int y, int x) {
  /* Initialize the player */
  if (player == NULL)
    player = actor_create(y, x, "player");
  player->y = y, player->x = x;
  DUNGEON[y][x].resident = player;
  CAMERA_X = player->x, CAMERA_Y = player->y;
  LOOK_X = player->x, LOOK_Y = player->y;
  player->name = PLAYER_NAME;
  player->IS_SEEN = 1;
}

void handle_scroll(int dy, int dx) {
  CAMERA_Y += 4*dy;
  CAMERA_X += 4*dx;
}

void handle_look(int dy, int dx) {
  if (CHK_VISIBLE(LOOK_Y+dy, LOOK_X+dx)) {
    LOOK_Y += dy;
    LOOK_X += dx;
  }
}

void handle_inv(int dy, int dx) {
  INV_POS += dy;
  if (INV_POS < 0)
    INV_POS = TCOD_list_size(*(player->inventory))-1;
  else if (INV_POS >= TCOD_list_size(*(player->inventory)))
    INV_POS = 0;
}

int attempt_move(int dy, int dx) {
  ACTOR *a;

  if (can_move(player, dy, dx)) {
    actor_move(player, dy, dx);
    return 1;
  }
  if ((a = DUNGEON[player->y+dy][player->x+dx].resident) != NULL) {
    actor_attack(player, a);
    return 1;
  }
  return 0;
}

/* Possible modes:
   - Action, Examine, Scroll, Inventory, Message, Equip
   Action Mode:
   Tries to perform a player action corresponding to the pressed key.
   Returns 1 if an action was peformed. */

int handle_direction(dy, dx) {
  switch (INPUT_MODE) {
	  case INPUT_ACTION:
		  return attempt_move(dy, dx);
		  break;
	  case INPUT_SCROLL:
		  handle_scroll(dy, dx);
		  return 0;
		  break;
	  case INPUT_LOOK:
		  handle_look(dy, dx);
		  return 0;
		  break;
	  case INPUT_INVENTORY:
		  handle_inv(dy, dx);
		  return 0;
		  break;
	  default:
		  return 0;
  }
}

int handle_input(TCOD_event_t ev, TCOD_keycode_t key, char ch, int ctrl, int m_x, int m_y) {
  int ret = 0, i;

  /* Change input modes */
  if ((INPUT_MODE != INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && key==TCODK_ESCAPE)
    INPUT_MODE = INPUT_ACTION; /* Escape -> default screen */
  else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && key==TCODK_ESCAPE) {
    return -1; /* Escape -> Main Menu */
  }
  else if ((INPUT_MODE == INPUT_ACTION) && ctrl)
    INPUT_MODE = INPUT_SCROLL;
  else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && ch=='x')
    INPUT_MODE = INPUT_LOOK;
  else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && ch=='i')
    INPUT_MODE = INPUT_INVENTORY;
  else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && ch=='m')
    INPUT_MODE = INPUT_MESSAGE;
  else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && ch=='e')
    INPUT_MODE = INPUT_EQUIP;
  else if ((INPUT_MODE == INPUT_ACTION || INPUT_MODE == INPUT_LOOK) && ev == TCOD_EVENT_MOUSE_PRESS) {
    m_y += player->y-CON_HEIGHT/2;
    m_x += player->x-(CON_WIDTH-UI_WIDTH)/2;
    if (CHK_VISIBLE(m_y, m_x)) {
      if (INPUT_MODE == INPUT_LOOK && LOOK_Y == m_y && LOOK_X == m_x)
		INPUT_MODE = INPUT_ACTION;
      else
		INPUT_MODE = INPUT_LOOK;
      LOOK_Y = m_y;
      LOOK_X = m_x;
    }
  }
  else if ((INPUT_MODE == INPUT_SCROLL) && !ctrl)
    INPUT_MODE = INPUT_ACTION;
  else if ((INPUT_MODE == INPUT_LOOK) && ev == TCOD_EVENT_KEY_PRESS && ch=='x') {
    INPUT_MODE = INPUT_ACTION;
  }
  else if ((INPUT_MODE == INPUT_INVENTORY) && ev == TCOD_EVENT_KEY_PRESS && ch=='i') {
    INPUT_MODE = INPUT_ACTION;
  }
  else if ((INPUT_MODE == INPUT_MESSAGE) && ev == TCOD_EVENT_KEY_PRESS && ch=='m')
    INPUT_MODE = INPUT_ACTION;
  else if ((INPUT_MODE == INPUT_EQUIP) && ev == TCOD_EVENT_KEY_PRESS && ch=='e')
    INPUT_MODE = INPUT_ACTION;

  /* Check key release */
  if (ev == TCOD_EVENT_KEY_RELEASE) {
    if (key == TCODK_CONTROL) {
      CAMERA_Y = player->y;
      CAMERA_X = player->x;
    }
    return 0;
  }

  /* Temporarily increment TURN_COUNT so that any messages that
     are generated are displayed properly. */
  TURN_COUNT++;

  /* Key was pressed */
  switch (key) {
  case TCODK_KP8:
  case TCODK_UP:
    ret = handle_direction(-1, 0);
    break;
  case TCODK_RIGHT:
  case TCODK_KP6:
    ret = handle_direction(0, 1);
    break;
  case TCODK_KP2:
  case TCODK_DOWN:
    ret = handle_direction(1, 0);
    break;
  case TCODK_KP4:
  case TCODK_LEFT:
    ret = handle_direction(0, -1);
    break;
  case TCODK_KP9:
    ret = handle_direction(-1, 1);
    break;
  case TCODK_KP3:
    ret = handle_direction(1, 1);
    break;
  case TCODK_KP1:
    ret = handle_direction(1, -1);
    break;
  case TCODK_KP7:
    ret = handle_direction(-1, -1);
    break;
  case TCODK_KP5:  /* take up turn doing nothing */
    ret = (INPUT_MODE == INPUT_ACTION);
    break;
  case TCODK_PRINTSCREEN:
  case TCODK_F12:
    TURN_COUNT--;
    TCOD_sys_save_screenshot(NULL);
    message_add(string_create(1, "A screenshot has been saved into the working directory"), ".");
    TURN_COUNT++;
    break;
  case TCODK_CHAR:
    if (INPUT_MODE == INPUT_INVENTORY) {
      i = ch-'a';
      if (i >= 0 && i < TCOD_list_size(*(player->inventory)))
	INV_POS = i;
    }
    switch (ch) {
    case 'g': /* pickup item */
    case ',':
      if (INPUT_MODE == INPUT_ACTION) {
	if (item_get_top(player->y, player->x) != NULL) {
	  actor_pickup(player, -1);
	  ret = 1;
	}
      }
      break;
    case 'd':
      if (INPUT_MODE == INPUT_INVENTORY) /* don't drop with 'd' in inventory */
	break;
    case 'D': /* drop item */
      if (INPUT_MODE == INPUT_ACTION) {
	if (TCOD_list_size(*(player->inventory)) > 0) {
	  actor_drop(player, -1);
	  ret = 1;
	}
      }
      else if (INPUT_MODE == INPUT_INVENTORY) {
	if (TCOD_list_size(*(player->inventory)) > INV_POS) {
	  actor_drop(player, INV_POS);
	  ret = 1;
	}
      }
      break;
    case 'w': /* wield item */
      if (INPUT_MODE == INPUT_INVENTORY) {
	if (TCOD_list_size(*(player->inventory)) > INV_POS) {
	  actor_wield(player, INV_POS);
	  ret = 1;
	}
      }
      break;
    case '<':
      if (INPUT_MODE == INPUT_ACTION) {
	if (DUNGEON[player->y][player->x].type == TILE_STAIRS_UP) {
	  /* Take the stairs up */
	  dungeon_next();
	  ret = 1;
	}
	else
	  ;
      }
      break;
    case '>':
      if (INPUT_MODE == INPUT_ACTION) {
	if (DUNGEON[player->y][player->x].type == TILE_STAIRS_DOWN) {
	  /* Take the stairs down */
	  dungeon_next();
	  ret = 1;
	}
	else
	  ;
      }
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }

  /* Update camera to center on player */
  if (INPUT_MODE == INPUT_ACTION) {
    CAMERA_X = player->x;
    CAMERA_Y = player->y;
  }
  /* If move was made, look coordinates should be reset */
  if (ret) {
    LOOK_X = player->x;
    LOOK_Y = player->y;
  }

  TURN_COUNT--;
  return ret;
}
