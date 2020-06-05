//
// Created by geo on 03.06.2020.
//

#include <SFML/Graphics.h>
#include <stdio.h>

#ifndef CHECKERS_CHECKERS_H
#define CHECKERS_CHECKERS_H

// region Settings

// Field size
#define FIELD_WIDTH 8
#define FIELD_HEIGHT 8

// Checkers colors
#define CHECKERS_BLACK_COLOR (sfColor)sfColor_fromRGB(0x10, 0x10, 0x10)
#define CHECKERS_WHITE_COLOR (sfColor)sfColor_fromRGB(0xF0, 0xF0, 0xF0)
#define CHECKERS_AVAILABLE_COLOR (sfColor)sfColor_fromRGB(0xFF, 0xFF, 0xFF)

// Field colors
#define CHECKERS_FIELD_BLACK_COLOR (sfColor)sfColor_fromRGB(0x60, 0x60, 0x60)
#define CHECKERS_FIELD_WHITE_COLOR (sfColor)sfColor_fromRGB(0xD0, 0xD0, 0xD0)

// endregion

// region Types

/** All possible states of checker */
enum CheckersType {
    black = 'b', white = 'w', none = ' ', available = '_'
};

/** Type provides you with properties of checker */
struct Checker {
    enum CheckersType type;
    int isBig;
};

// endregion

// region Methods

/** Fill field with default position of checkers */
void Checkers_create();

/** Print current state of field to console */
void Checkers_log();

/** Draw field + checkers states on screen
 * @param renderWindow Window
 * @param shape Rectangle shape that will be drawn
 * */
void Checkers_draw(sfRenderWindow *renderWindow, sfRectangleShape *shape);

/** Convert field fieldPosition to screen one
 * @param renderWindow Window
 * @param fieldPosition Field coordinates
 * @returns Screen coordinates
 * */
sfVector2f Checkers_scaleToScreen(sfRenderWindow *renderWindow, sfVector2i fieldPosition);

/** Convert mouse click position to field coordinates
 * @param renderWindow Window
 * @param screenPosition Click position
 * @returns Field coordinates
 * */
sfVector2i Checkers_scaleFromScreen(sfRenderWindow *renderWindow, sfVector2i screenPosition);

/** Update scene
 * @param position Click position
 * */
void Checkers_react(sfVector2i position);

// endregion

#endif //CHECKERS_CHECKERS_H
