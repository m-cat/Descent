#include <libtcod.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "defs.h"
#include "util.h"
#include "player.h"
#include "init.h"

/* Define item_parser listener */

ITEM *current_item;

bool item_parser_new_struct(TCOD_parser_struct_t str, const char *name) {
  current_item = malloc(sizeof(ITEM));
  current_item->name = strdup(name);
  return true;
}
bool item_parser_flag(const char *name) {
  return true;
}
bool item_parser_property(const char *name, TCOD_value_type_t type, TCOD_value_t value) {
  if (strcmp(name, "type") == 0)
    current_item->type = value.i;
  else if (strcmp(name, "ch") == 0)
    current_item->ch = value.c;
  else if (strcmp(name, "col") == 0)
    current_item->col = value.col;
  return true;
}
bool item_parser_end_struct(TCOD_parser_struct_t str, const char *name) {
  TCOD_list_push(item_type_list,(const void *)current_item);
  return true;
}
void item_parser_error(const char *msg) {
  fprintf(stderr,msg);
  exit(1);
}
TCOD_parser_listener_t item_listener = {
  item_parser_new_struct,
  item_parser_flag,
  item_parser_property,
  item_parser_end_struct,
  item_parser_error
};

/* Do all initializations for the game */
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
  item_type_list = TCOD_list_allocate(100); /* initially allocate space for 100 items */
  message_list = TCOD_list_allocate(MESSAGE_LIST_LEN);

  /* Parse config files */
  TCOD_parser_t item_parser = TCOD_parser_new(); /* defined in defs.h */
  TCOD_parser_struct_t item_struct = TCOD_parser_new_struct(item_parser, "item_type");
  TCOD_struct_add_property(item_struct, "type", TCOD_TYPE_INT, 1);
  TCOD_struct_add_property(item_struct, "ch", TCOD_TYPE_CHAR, 1);
  TCOD_struct_add_property(item_struct, "col", TCOD_TYPE_COLOR, 1);
  TCOD_parser_run(item_parser, "data/items.conf", &item_listener);
  TCOD_parser_delete(item_parser);

  /* Initialize misc */
  TCOD_sys_set_fps(FPS);
  TCOD_console_set_background_flag(NULL, TCOD_BKGND_SET);

  /* Initialize the player */
  player.ch = '@';
  player.inventory = malloc(sizeof(TCOD_list_t));
  *(player.inventory) = TCOD_list_allocate(100);

  return 0;
}
