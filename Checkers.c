//
// Created by geo on 03.06.2020.
//

#include "Checkers.h"

/** Field containing all checkers */
struct Checker Checkers_field[FIELD_WIDTH][FIELD_HEIGHT];

/** Create all for drawing */
void Checkers_create() {
    // Generate field
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            if ((i + j) % 2) {
                if (j < 3) // Set black checkers at top
                    Checkers_field[i][j].type = black;
                else if (j >= FIELD_HEIGHT - 3) // Set white checkers at bottom
                    Checkers_field[i][j].type = white;
                else // All other positions are empty
                    Checkers_field[i][j].type = none;
            } else {
                Checkers_field[i][j].type = none;
            }

            // All checkers are small by default
            Checkers_field[i][j].isBig = 0;
        }
    }
}

/** Print field to console as chars */
void Checkers_log() {
    // Print checkers
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            // Print checker as char
            printf("%c ", Checkers_field[j][i].type);
        }
        // Return + new line
        printf("\n");
    }
}

/** Scale vector to draw on screen
 * @param renderWindow Window
 * @param fieldPosition Position on field
 * @returns Position on screen
 * */
sfVector2f Checkers_scaleToScreen(sfRenderWindow *renderWindow, sfVector2i fieldPosition) {
    // Save window size to save resources
    sfVector2u win_size = sfRenderWindow_getSize(renderWindow);

    /* Scaled vector = {
     *      x' = x / width * screen width
     *      y' = y / height * screen height
     * } -- It convert field positions to draw on screen */
    return (sfVector2f) {.x = (float) fieldPosition.x / (float) FIELD_WIDTH * (float) win_size.x,
            .y = (float) fieldPosition.y / (float) FIELD_HEIGHT * (float) win_size.y};
}

/** Scale vector to get clicks from screen
 * @param renderWindow Window
 * @param screenPosition Position on screen
 * @returns Position on field
 * */
sfVector2i Checkers_scaleFromScreen(sfRenderWindow *renderWindow, sfVector2i screenPosition) {
    // Save window size to save resources
    sfVector2u win_size = sfRenderWindow_getSize(renderWindow);

    /* Scaled vector = {
     *      x' = x / screen width * width
     *      y' = y / screen height *  height
     * } -- It convert click position to field one */
    return (sfVector2i) {.x = (int) ((float) screenPosition.x / (float) win_size.x * FIELD_WIDTH),
            .y = (int) ((float) screenPosition.y / (float) win_size.y * FIELD_HEIGHT)};
}

/** Draw field on given window
 * @param renderWindow Window
 * @param shape Rectangle shape
 * */
void Checkers_draw(sfRenderWindow *renderWindow, sfRectangleShape *shape) {
    // Calculate shape size (required for drawing, will be set as shape width + height)
    sfVector2f tile_size = Checkers_scaleToScreen(renderWindow, (sfVector2i) {.x = 1, .y = 1});

    // Set shape width & height
    sfRectangleShape_setSize(shape, tile_size);

    // Draw field
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            // Calculate shape next position (based on tile_size)
            sfVector2f tile_position = (sfVector2f) {.x = tile_size.x * (float) i, .y = tile_size.y * (float) j};

            // Set shape a new position
            sfRectangleShape_setPosition(shape, tile_position);

            // Use different colors for background field
            if ((i + j) % 2)
                sfRectangleShape_setFillColor(shape, CHECKERS_FIELD_BLACK_COLOR);
            else
                sfRectangleShape_setFillColor(shape, CHECKERS_FIELD_WHITE_COLOR);

            // Draw field part to screen
            sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);
        }
    }

    // Draw checkers
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            // Calculate shape next position (based on tile_size)
            sfVector2f tile_position = (sfVector2f) {.x = tile_size.x * (float) i, .y = tile_size.y * (float) j};

            // Set shape a new position
            sfRectangleShape_setPosition(shape, tile_position);

            // Depending on state of part in Checkers_field draws it with different colors
            switch (Checkers_field[i][j].type) {
                case black:
                    sfRectangleShape_setFillColor(shape, CHECKERS_BLACK_COLOR);
                    sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);
                    break;

                case white:
                    sfRectangleShape_setFillColor(shape, CHECKERS_WHITE_COLOR);
                    sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);
                    break;

                case available:
                    sfRectangleShape_setFillColor(shape, CHECKERS_AVAILABLE_COLOR);
                    sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);

                default:
                    break;
            }
        }
    }
}

/** What check is current now */
sfVector2i Checkers_checkedPosition;

/** Last type cant step again */
enum CheckersType Checkers_lastCheck;

/** Checks if point is belong to field
 * @returns Does point belongs to field
 * */
int Checkers_isInField(sfVector2i pos) {
    return pos.x >= 0 && pos.y >= 0 &&
           pos.x < FIELD_WIDTH && pos.y < FIELD_HEIGHT;
}

/** Sum up field position + offset
 * @param position Field position
 * @param offset offset
 * @returns New field coordinates with offset
 * */
sfVector2i Checkers_plus(sfVector2i position, sfVector2i offset) {
    return (sfVector2i) {.x = position.x + offset.x, .y = position.y + offset.y};
}

/** Multiply field position with some coefficient
 * @param position Field position
 * @param coefficient Coefficient
 * */
sfVector2i Checkers_mul(sfVector2i position, sfVector2i coefficient) {
    return (sfVector2i) {.x = position.x * coefficient.x, .y = position.y * coefficient.y};
}

/** Make checker available in cross part
 * @param position Field position
 * @param coefficient Two values in range (-1 - 1) - direction of cross part
 * */
void Checkers_makeAvailableLoop(sfVector2i position, sfVector2i coefficient) {
    int offset = 0; // Counter

    int wasChecker = 0;

    sfVector2i nextAvailablePoint; // Next available point
    while (1) {
        offset++; // Start incrementing offset while it is belong screen

        // Calculate new point using offset
        nextAvailablePoint = Checkers_plus(position,
                                           Checkers_mul((sfVector2i) {.x = offset, .y = offset}, coefficient));

        if ((Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].type == black ||
             Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].type == white) && !wasChecker)
            wasChecker = 1;

        // Continue loop if point belongs to field & point type is none
        if (Checkers_isInField(nextAvailablePoint) &&
            Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].type == none)
            Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].type = available;
        else
            break;
    }
}

/** Make available cross with center in position (for big checkers)
 * @param position Field position
 * */
void Checkers_makeAvailableCross(sfVector2i position) {
    // Left - Bottom part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) {.x = 1, .y = 1});

    // Left - Top part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) {.x = 1, .y = -1});

    // Right - Bottom part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) {.x = -1, .y = 1});

    // Right - Top part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) {.x = -1, .y = -1});
}

/** Make available next two parts (for small checkers)
 * @param position Field position
 * @param type Checker type (White | Black)
 * */
void Checkers_makeAvailableNearest(sfVector2i position, enum CheckersType type) {
    int coefficient;

    // if type is black or white continue
    // else stop
    if (type == black)
        coefficient = 1;
    else if (type == white)
        coefficient = -1;
    else
        return;

    int offsetX = 1;

    // Make available:
    // Upper checker
    if (Checkers_field[position.x + offsetX][position.y + coefficient].type == none)
        Checkers_field[position.x + offsetX][position.y + coefficient].type = available;
    else if (Checkers_field[position.x + offsetX][position.y + coefficient].type == black ||
             Checkers_field[position.x + offsetX][position.y + coefficient].type == white)
        if (Checkers_field[position.x + 2 * offsetX][position.y + 2 * coefficient].type == none)
            Checkers_field[position.x + 2 * offsetX][position.y + 2 * coefficient].type = available;
    // Down checker
    if (Checkers_field[position.x - offsetX][position.y + coefficient].type == none)
        Checkers_field[position.x - offsetX][position.y + coefficient].type = available;
    else if (Checkers_field[position.x - offsetX][position.y + coefficient].type == black ||
             Checkers_field[position.x - offsetX][position.y + coefficient].type == white)
        if (Checkers_field[position.x - 2 * offsetX][position.y + 2 * coefficient].type == none)
            Checkers_field[position.x - 2 * offsetX][position.y + 2 * coefficient].type = available;
}

int Checkers_removeEatenChecker(sfVector2i start, sfVector2i stop, enum CheckersType type) {
    if (!(Checkers_isInField(start) && Checkers_isInField(stop)))
        return (int) NULL;

    sfVector2i coefficient =
            {.x = stop.x - start.x > 0 ? 1 : -1,
                    .y = stop.y - start.y > 0 ? 1 : -1};

    sfVector2i current = start;

    int wasEaten = 0;

    while (current.x != stop.x && current.y != stop.y) {
        current.x += coefficient.x;
        current.y += coefficient.y;
        if (Checkers_field[current.x][current.y].type == type) {
            Checkers_field[current.x][current.y].type = none;
            wasEaten = 1;
        }
    }

    return wasEaten;
}

/** Change current check */
void Checkers_react(sfVector2i position) {
    if (Checkers_field[position.x][position.y].type == black ||
        Checkers_field[position.x][position.y].type == white) {
        // Clear old available checkers
        for (int i = 0; i < FIELD_WIDTH; ++i) {
            for (int j = 0; j < FIELD_HEIGHT; ++j) {
                if (Checkers_field[i][j].type == available)
                    Checkers_field[i][j].type = none;
            }
        }

        // Makes click position (in field) current
        Checkers_checkedPosition = position;

        if (Checkers_field[position.x][position.y].isBig) {
            Checkers_makeAvailableCross(position); // Draw cross
            Checkers_makeAvailableNearest(position, black); // Draw small crosses
            Checkers_makeAvailableNearest(position, white); // Draw small crosses
        } else
            Checkers_makeAvailableNearest(position, Checkers_field[position.x][position.y].type); // Draw small crosses
    } else if (Checkers_field[position.x][position.y].type == available &&
               Checkers_lastCheck != Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].type) {
        int wasEaten = 0;

        if (Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].type == white)
            wasEaten = Checkers_removeEatenChecker(position, Checkers_checkedPosition, black);
        else if (Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].type == black)
            wasEaten = Checkers_removeEatenChecker(position, Checkers_checkedPosition, white);

        // Replace available position with black or white checker
        Checkers_field[position.x][position.y] =
                Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y];

        // Make old checked position empty
        Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].type = none;

        if (!wasEaten)
            // Change next step`s type
            Checkers_lastCheck = Checkers_field[position.x][position.y].type;

        // If check if on the edge of field, make it big
        if (Checkers_lastCheck == white && position.y == 0 ||
            Checkers_lastCheck == black && position.y == FIELD_HEIGHT - 1)
            Checkers_field[position.x][position.y].isBig = 1;
    }
}
