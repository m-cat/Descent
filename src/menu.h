#ifndef MENU_H
#define MENU_H

enum MENU_SCREEN
{
    MENU_MAIN,
    MENU_NAME
};

int draw_menu(enum MENU_SCREEN menu);

int handle_menu();

#endif
