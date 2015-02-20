#include <libtcod.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "defs.h"
#include "priority.h"
#include "util.h"
#include "dungeon.h" 
#include "player.h"
#include "init.h"

ACTOR *current_actor;
ITEM *current_item;
DUNGEON_BLOCK *current_block;

/* Define actor_parser listener */

bool actor_parser_new_struct(TCOD_parser_struct_t str, const char *name) {
  current_actor = calloc(sizeof(ACTOR), 1);
  current_actor->name = strdup(name);
  return true;
}
bool actor_parser_flag(const char *name) {
  return true;
}
bool actor_parser_property(const char *name, TCOD_value_type_t type, TCOD_value_t value) {
  if (strcmp(name, "art") == 0)
    current_actor->art = strdup(value.s);
  else if (strcmp(name, "type") == 0)
    current_actor->type = value.i;
  else if (strcmp(name, "ch") == 0)
    current_actor->ch = value.c;
  else if (strcmp(name, "col") == 0)
    current_actor->col = value.col;
  else if (strcmp(name, "spd") == 0)
    current_actor->spd = value.i;
  return true;
}
bool actor_parser_end_struct(TCOD_parser_struct_t str, const char *name) {
  TCOD_list_push(actor_type_list,(const void *)current_actor);
  return true;
}
void actor_parser_error(const char *msg) {
  fprintf(stderr,msg);
  exit(1);
}
TCOD_parser_listener_t actor_listener = {
  actor_parser_new_struct,
  actor_parser_flag,
  actor_parser_property,
  actor_parser_end_struct,
  actor_parser_error
};

/* Define item_parser listener */

bool item_parser_new_struct(TCOD_parser_struct_t str, const char *name) {
  current_item = calloc(sizeof(ITEM), 1);
  current_item->name = strdup(name);
  return true;
}
bool item_parser_flag(const char *name) {
  return true;
}
bool item_parser_property(const char *name, TCOD_value_type_t type, TCOD_value_t value) {
  if (strcmp(name, "art") == 0)
    current_item->art = strdup(value.s);
  else if (strcmp(name, "type") == 0)
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

/* Define block_parser listener */

bool block_parser_new_struct(TCOD_parser_struct_t str, const char *name) {
  current_block = calloc(sizeof(DUNGEON_BLOCK), 1);
  current_block->name = strdup(name);
  return true;
}
bool block_parser_flag(const char *name) {
  return true;
}
bool block_parser_property(const char *name, TCOD_value_type_t type, TCOD_value_t value) {
  if (strcmp(name, "art") == 0)
    current_block->art = strdup(value.s);
  else if (strcmp(name, "type") == 0)
    current_block->type = value.i;
  else if (strcmp(name, "ch") == 0)
    current_block->ch = value.c;
  else if (strcmp(name, "col_vis") == 0)
    current_block->col_vis = value.col;
  else if (strcmp(name, "col_nonvis") == 0)
    current_block->col_nonvis = value.col;
  else if (strcmp(name, "transparent") == 0)
    current_block->TRANSPARENT = value.i;
  else if (strcmp(name, "passable") == 0)
    current_block->PASSABLE = value.i;
  return true;
}
bool block_parser_end_struct(TCOD_parser_struct_t str, const char *name) {
  if (strcmp(current_block->name, "wall") == 0)
    block_copy(&block_wall, current_block);
  else if (strcmp(current_block->name, "floor") == 0)
    block_copy(&block_floor, current_block);
  TCOD_list_push(block_type_list,(const void *)current_block);
  return true;
}
void block_parser_error(const char *msg) {
  fprintf(stderr,msg);
  exit(1);
}
TCOD_parser_listener_t block_listener = {
  block_parser_new_struct,
  block_parser_flag,
  block_parser_property,
  block_parser_end_struct,
  block_parser_error
};

void init_parse_config() {
  TCOD_parser_t actor_parser = TCOD_parser_new();
  TCOD_parser_struct_t actor_struct =
    TCOD_parser_new_struct(actor_parser, "actor_type");
  TCOD_struct_add_property(actor_struct, "art", TCOD_TYPE_STRING, 1);
  TCOD_struct_add_property(actor_struct, "type", TCOD_TYPE_INT, 1);
  TCOD_struct_add_property(actor_struct, "ch", TCOD_TYPE_CHAR, 1);
  TCOD_struct_add_property(actor_struct, "col", TCOD_TYPE_COLOR, 1);
  TCOD_struct_add_property(actor_struct, "spd", TCOD_TYPE_INT, 1);
  TCOD_parser_run(actor_parser, "data/actors.conf", &actor_listener);
  TCOD_parser_delete(actor_parser);

  TCOD_parser_t item_parser = TCOD_parser_new();
  TCOD_parser_struct_t item_struct =
    TCOD_parser_new_struct(item_parser, "item_type");
  TCOD_struct_add_property(item_struct, "art", TCOD_TYPE_STRING, 1);
  TCOD_struct_add_property(item_struct, "type", TCOD_TYPE_INT, 1);
  TCOD_struct_add_property(item_struct, "ch", TCOD_TYPE_CHAR, 1);
  TCOD_struct_add_property(item_struct, "col", TCOD_TYPE_COLOR, 1);
  TCOD_parser_run(item_parser, "data/items.conf", &item_listener);
  TCOD_parser_delete(item_parser);

  TCOD_parser_t block_parser = TCOD_parser_new();
  TCOD_parser_struct_t block_struct =
    TCOD_parser_new_struct(block_parser, "block_type");
  TCOD_struct_add_property(block_struct, "art", TCOD_TYPE_STRING, 1);
  TCOD_struct_add_property(block_struct, "type", TCOD_TYPE_INT, 1);
  TCOD_struct_add_property(block_struct, "ch", TCOD_TYPE_CHAR, 1);
  TCOD_struct_add_property(block_struct, "col_vis", TCOD_TYPE_COLOR, 1);
  TCOD_struct_add_property(block_struct, "col_nonvis", TCOD_TYPE_COLOR, 1);
  TCOD_struct_add_property(block_struct, "transparent", TCOD_TYPE_INT, 1);
  TCOD_struct_add_property(block_struct, "passable", TCOD_TYPE_INT, 1);
  TCOD_parser_run(block_parser, "data/blocks.conf", &block_listener);
  TCOD_parser_delete(block_parser);
 }

/* Do all initializations for the game */
int init_all() {
  int i;

  /* Initialize TCOD console */
  TCOD_console_init_root(CON_WIDTH, CON_HEIGHT, GAME_NAME,
			 0, TCOD_RENDERER_SDL);

  /* Initialize data structures */
  DUNGEON = malloc(MAX_HEIGHT * sizeof(DUNGEON_BLOCK *));
  for (i = 0; i < MAX_HEIGHT; i++) {
    DUNGEON[i] = calloc(MAX_WIDTH, sizeof(DUNGEON_BLOCK));
    assert(DUNGEON[i] != NULL);
  }
  fov_map = TCOD_map_new(MAX_WIDTH, MAX_HEIGHT);
  actor_type_list = TCOD_list_allocate(128);
  item_type_list = TCOD_list_allocate(128); /* allocate space for 128 items */
  block_type_list = TCOD_list_allocate(32);
  message_list = TCOD_list_allocate(MESSAGE_LIST_LEN);
  message_turn_list = TCOD_list_allocate(MESSAGE_LIST_LEN);
  message_add(string_create(2,"Welcome to ", GAME_NAME), "!");
  actor_queue = priq_new(sizeof(ACTOR *));
  temp_queue = priq_new(sizeof(ACTOR *));

  /* Parse config files */
  init_parse_config();

  /* Initialize misc */
  TCOD_sys_set_fps(FPS);
  TCOD_console_set_background_flag(NULL, TCOD_BKGND_SET);
  TCOD_console_set_keyboard_repeat(0,0);


  return 0;
}
