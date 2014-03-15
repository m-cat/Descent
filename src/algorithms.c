#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <libtcod.h>
#include "defs.h"
#include "util.h"
#include "player.h"
#include "algorithms.h"

#define VIS_SET(x, y) (SET_VISIBLE(y, x, 1),	\
		       SET_EXPLORED(y, x, 1))
#define VIS_CLR(x, y) (SET_VISIBLE(y, x, 0))

#define CALC_DIST(x0, y0, x1, y1) sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))
#define FLT_LTE(n1, n2) ((n1) < (n2) || abs((n2)-(n1)) < .001)

void calc_fov() {
  int i, j;
  ACTOR *a;
  char *subject;

  TCOD_map_compute_fov(fov_map, player->x, player->y, FOV_RADIUS, 1, FOV_SHADOW);
  for (i = MAX(DUNGEON_Y, CAMERA_Y-CON_HEIGHT+1);
       i < MIN(DUNGEON_Y+CURRENT_HEIGHT, CAMERA_Y+CON_HEIGHT); i++)
    for (j = MAX(DUNGEON_X, CAMERA_X-CON_WIDTH+1); 
	 j < MIN(DUNGEON_X+CURRENT_WIDTH, CAMERA_X+CON_WIDTH); j++) {
      TCOD_map_is_in_fov(fov_map, j, i) ? VIS_SET(j, i) : VIS_CLR(j, i);
      if (CHK_VISIBLE(i, j) && DUNGEON[i][j].resident != NULL) {
	a = DUNGEON[i][j].resident;
	if (a->IS_SEEN == 0) {
	  subject = subject_form(a->art, 1, a->name);
	  message_add(string_create(2,"You see ", subject), ".");
	  free(subject);
	  a->IS_SEEN = 1;
	}
      }
    }
}
