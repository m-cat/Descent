#include <libtcod.h>
#include <stdlib.h>
#include <assert.h>
#include "defs.h"
#include "player.h"
#include "init.h"

int init_all() {
  int i;

  /* Initialize TCOD console */
  TCOD_console_init_root(CON_WIDTH, CON_HEIGHT, GAME_NAME, 0, TCOD_RENDERER_SDL);

  /* Initialize data structures */
  DUNGEON = malloc(MAX_HEIGHT * sizeof(DUNGEON_BLOCK *));
  for (i = 0; i < MAX_HEIGHT; i++) {
    DUNGEON[i] = calloc(MAX_WIDTH, sizeof(DUNGEON_BLOCK));
    assert(DUNGEON[i] != NULL);
  }
  fov_map = TCOD_map_new(MAX_WIDTH, MAX_HEIGHT);

  /* Parse config files */
  item_parser = TCOD_parser_new(); /* defined in defs.h */
  TCOD_parser_struct_t item_struct = TCOD_parser_new_struct(item_parser, "item_type");
  TCOD_struct_add_property(item_struct, "type", TCOD_TYPE_INT, 1);
  TCOD_struct_add_property(item_struct, "ch", TCOD_TYPE_CHAR, 1);
  TCOD_parser_run(item_parser, "data/items.conf", NULL);

  /* Initialize misc */
  TCOD_sys_set_fps(FPS);
  TCOD_console_set_background_flag(NULL, TCOD_BKGND_SET);

  /* Initialize the player */
  player.ch = '@';

  return 0;
}
