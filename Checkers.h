#include <SFML/Graphics.h>
#include <stdio.h>

#ifndef CHECKERS_CHECKERS_H
#define CHECKERS_CHECKERS_H

#define FIELD_WIDTH 8
#define FIELD_HEIGHT 8

#define CHECKERS_BLACK_COLOR (sfColor)sfColor_fromRGB(0x10, 0x10, 0x10)
#define CHECKERS_WHITE_COLOR (sfColor)sfColor_fromRGB(0xF0, 0xF0, 0xF0)
#define CHECKERS_AVAILABLE_COLOR (sfColor)sfColor_fromRGB(0xFF, 0xFF, 0xFF)

#define CHECKERS_FIELD_BLACK_COLOR (sfColor)sfColor_fromRGB(0x60, 0x60, 0x60)
#define CHECKERS_FIELD_WHITE_COLOR (sfColor)sfColor_fromRGB(0xD0, 0xD0, 0xD0)

enum CheckersState {
    black = 'b', white = 'w', none = ' ', available = '_'
};

struct Checker {
    enum CheckersState state;
    int isBig;
};

void Checkers_create();

void Checkers_log();
void Checkers_draw(sfRenderWindow* renderWindow, sfRectangleShape* shape);

sfVector2f Checkers_scaleToScreen(sfRenderWindow* renderWindow, sfVector2i fieldPosition);
sfVector2i Checkers_scaleFromScreen(sfRenderWindow* renderWindow, sfVector2i screenPosition);

void Checkers_react(sfVector2i position);

#endif
