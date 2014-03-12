#include <libtcod.h>
#include "defs.h"
#include "items.h"
#include "actor.h"
#include "player.h"

ACTOR player;


void player_place(int y, int x) {
  player.x = x;
  player.y = y;
  CAMERA_X = player.x, CAMERA_Y = player.y;
  LOOK_X = player.x, LOOK_Y = player.y;
  DUNGEON[player.y][player.x].resident = &player;
}

void handle_scroll(int dy, int dx) {
  CAMERA_Y += dy;
  CAMERA_X += dx;
}
void handle_look(int dy, int dx) {
  if (CHK_VISIBLE(LOOK_Y+dy, LOOK_X+dx)) {
    LOOK_Y += dy;
    LOOK_X += dx;
  }
}

/* Possible modes:
   - Action, Examine, Scroll, Inventory
   Action Mode:
   Tries to perform a player action corresponding to the pressed key.
   Returns 1 if an action was peformed. */
#define move_if_can(p, dy, dx)					\
  (can_move(&p, dy, dx) ? (actor_move(&p, dy, dx), 1) : 0)
#define handle_direction(dy, dx)					\
  ((INPUT_MODE==INPUT_ACTION) ? move_if_can(player, dy, dx) :		\
   ((INPUT_MODE==INPUT_SCROLL) ? (handle_scroll(dy, dx), 0) :		\
    ((INPUT_MODE==INPUT_LOOK) ? (handle_look(dy, dx), 0) :		\
     0)))

int handle_input(TCOD_event_t ev, TCOD_keycode_t key, char ch, int ctrl,
		 int m_x, int m_y) {
  int ret = 0;

  /* Change input modes */
  if ((INPUT_MODE == INPUT_ACTION) && ctrl)
    INPUT_MODE = INPUT_SCROLL;
  else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && ch=='x')
    INPUT_MODE = INPUT_LOOK;
  else if ((INPUT_MODE == INPUT_ACTION) && ev == TCOD_EVENT_KEY_PRESS && ch=='i')
    INPUT_MODE = INPUT_INVENTORY;
  else if ((INPUT_MODE == INPUT_ACTION || INPUT_MODE == INPUT_LOOK)
	   && ev == TCOD_EVENT_MOUSE_PRESS) {
    m_y += player.y-CON_HEIGHT/2;
    m_x += player.x-(CON_WIDTH-UI_WIDTH)/2;
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

  /* Check key release */
  if (ev == TCOD_EVENT_KEY_RELEASE) {
    if (key == TCODK_CONTROL) {
      CAMERA_Y = player.y;
      CAMERA_X = player.x;
    }
    return 0;
  }
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
  case TCODK_KP5:
    ret = 1; /* take up turn doing nothing */
    break;
  case TCODK_PRINTSCREEN:
  case TCODK_F12:
    TCOD_sys_save_screenshot(NULL);
    break;
  case TCODK_CHAR:
    switch (ch) {
    case '.': /* pickup item */
      if (INPUT_MODE == INPUT_ACTION) {
	if (item_get_top(player.y, player.x) != NULL) {
	  actor_pickup(&player, player.y, player.x, -1);
	  ret = 1;
	}
      }
      break;
    case 'd': /* drop item */
      if (INPUT_MODE == INPUT_ACTION) {
	if (TCOD_list_size(*(player.inventory)) > 0) {
	  actor_drop(&player, player.y, player.x, -1);
	  ret = 1;
	}
      }
      break;
    case '>':
      if (INPUT_MODE == INPUT_ACTION) {
	if (DUNGEON[player.y][player.x].type == TILE_STAIRS_DOWN) {
	  /* Take the stairs down */
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
    CAMERA_X = player.x;
    CAMERA_Y = player.y;
  }
  /* If move was made, look coordinates should be reset */
  if (ret) {
    LOOK_X = player.x;
    LOOK_Y = player.y;
  }

  return ret;
}
