#include <libtcod.h>
#include <stdlib.h>
#include "defs.h"
#include "init.h"

int init_all() {
  int i, j;

  /* Initialize TCOD console */
  TCOD_console_init_root(CON_WIDTH, CON_HEIGHT, GAME_NAME, 0, TCOD_RENDERER_SDL);

  /* Initialize data structures */
  fov_map = TCOD_map_new(MAX_WIDTH, MAX_HEIGHT);

  /* Parse config files */
  TCOD_parser_t item_parser = TCOD_parser_new();
  TCOD_parser_struct_t ITEM_struct = 
    TCOD_parser_new_struct(item_parser, "ITEM");
  TCOD_parser_run(item_parser, "data/items", NULL);
  TCOD_parser_delete(item_parser);

  /* Initialize misc */
  TCOD_sys_set_fps(FPS);
  TCOD_console_set_background_flag(NULL, TCOD_BKGND_SET);

  return 0;
}
