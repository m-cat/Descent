#include <math.h>
#include "defs.h"
#include "util.h"
#include "player.h"
#include "algorithms.h"

#define SET_VIS(x, y) SET_BIT(DUNGEON[y][x].opt_tile, TILE_VISIBLE), \
    SET_BIT(DUNGEON[y][x].opt_tile, TILE_EXPLORED)

#define CALC_DIST(x0, y0, x1, y1) sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))

/* Calculates visible squares using Bresenham's algorithm */
void calc_fov_line(int x0, int y0, int x1, int y1) {
  int dx = abs(x1-x0);
  int dy = abs(y1-y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx-dy, e2;

  while (1) {
    SET_VIS(x0, y0);
    if (!CHK_BIT(DUNGEON[y0][x0].opt_tile, TILE_TRANSPARENT) ||
	(x0 == x1 && y0 == y1))
      break;
    e2 = 2*err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void calc_fov() {
  int i, j, sx, sy;
  /* Clear visibility */
  for (i = 0; i < CURRENT_HEIGHT; i++)
    for (j = 0; j < CURRENT_WIDTH; j++)
      CLR_BIT(DUNGEON[i][j].opt_tile, TILE_VISIBLE);
  /* Cast rays out in a circle */
  for (i = MAX(0, player.y-FOV_RADIUS); i < MIN(CURRENT_HEIGHT, player.y+FOV_RADIUS); i++)
    for (j = MAX(0, player.x-FOV_RADIUS); j < MIN(CURRENT_WIDTH, player.x+FOV_RADIUS); j++)
      if (!CHK_BIT(DUNGEON[i][j].opt_tile, TILE_VISIBLE) && CALC_DIST(j, i, player.x, player.y) <= FOV_RADIUS)
	calc_fov_line(player.x, player.y, j, i);
  /* Post-process, make more walls visible */
  for (i = MAX(0, player.y-FOV_RADIUS-1); i < MIN(CURRENT_HEIGHT, player.y+FOV_RADIUS-1); i++)
    for (j = MAX(0, player.x-FOV_RADIUS+1); j < MIN(CURRENT_WIDTH, player.x+FOV_RADIUS+1); j++) {
      sx = SIGN(player.x-j), sy = SIGN(player.y-i);
      if (!CHK_BIT(DUNGEON[i][j].opt_tile, TILE_TRANSPARENT)) {
	if ((CHK_BIT(DUNGEON[i+sy][j].opt_tile, TILE_TRANSPARENT) &&
	     CHK_BIT(DUNGEON[i+sy][j].opt_tile, TILE_VISIBLE)) ||
	     (CHK_BIT(DUNGEON[i][j+sx].opt_tile, TILE_TRANSPARENT) &&
	      CHK_BIT(DUNGEON[i][j+sx].opt_tile, TILE_VISIBLE)))
	    SET_VIS(i, j);
      }
    }
}
