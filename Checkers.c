#include "Checkers.h"

struct Checker Checkers_field[FIELD_WIDTH][FIELD_HEIGHT];

void Checkers_create() {
    // Generate field
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            if ((i + j) % 2) {
                if (j < 3) // Set black checkers at top
                    Checkers_field[i][j].state = black;
                else if (j >= FIELD_HEIGHT - 3) // Set white checkers at bottom
                    Checkers_field[i][j].state = white;
                else // All other positions are empty
                    Checkers_field[i][j].state = none;
            } else {
                Checkers_field[i][j].state = none;
            }

            Checkers_field[i][j].isBig = 0;
        }
    }
}

void Checkers_log() {
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            printf("%c ", Checkers_field[j][i].state);
        }
        printf("\n");
    }
}

sfVector2f Checkers_scaleToScreen(sfRenderWindow* renderWindow, sfVector2i fieldPosition) {
    sfVector2u win_size = sfRenderWindow_getSize(renderWindow);

    /* Scaled vector = {
     *      x' = x / width * screen width
     *      y' = y / height * screen height
     * } -- It convert field positions to draw on screen */
    return (sfVector2f) { .x = (float) fieldPosition.x / (float) FIELD_WIDTH * (float) win_size.x,
            .y = (float) fieldPosition.y / (float) FIELD_HEIGHT * (float) win_size.y };
}

sfVector2i Checkers_scaleFromScreen(sfRenderWindow* renderWindow, sfVector2i screenPosition) {
    sfVector2u win_size = sfRenderWindow_getSize(renderWindow);

    /* Scaled vector = {
     *      x' = x / screen width * width
     *      y' = y / screen height *  height
     * } -- It convert click position to field one */
    return (sfVector2i) { .x = (int) ((float) screenPosition.x / (float) win_size.x * FIELD_WIDTH),
            .y = (int) ((float) screenPosition.y / (float) win_size.y * FIELD_HEIGHT) };
}

void Checkers_draw(sfRenderWindow* renderWindow, sfRectangleShape* shape) {
    // Calculate shape size (required for drawing, will be set as shape width + height)
    sfVector2f tile_size = Checkers_scaleToScreen(renderWindow, (sfVector2i) { .x = 1, .y = 1 });
    sfRectangleShape_setSize(shape, tile_size);

    // Draw field
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            sfVector2f tile_position = (sfVector2f) { .x = tile_size.x * (float) i, .y = tile_size.y * (float) j };
            sfRectangleShape_setPosition(shape, tile_position);

            if ((i + j) % 2)
                sfRectangleShape_setFillColor(shape, CHECKERS_FIELD_BLACK_COLOR);
            else
                sfRectangleShape_setFillColor(shape, CHECKERS_FIELD_WHITE_COLOR);

            sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);
        }
    }

    // Draw checkers
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            sfVector2f tile_position = (sfVector2f) { .x = tile_size.x * (float) i, .y = tile_size.y * (float) j };

            sfRectangleShape_setPosition(shape, tile_position);

            // Depending on state of part in Checkers_field draws it with different colors
            switch (Checkers_field[i][j].state) {
            case black:sfRectangleShape_setFillColor(shape, CHECKERS_BLACK_COLOR);
                sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);
                break;

            case white:sfRectangleShape_setFillColor(shape, CHECKERS_WHITE_COLOR);
                sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);
                break;

            case available:sfRectangleShape_setFillColor(shape, CHECKERS_AVAILABLE_COLOR);
                sfRenderWindow_drawRectangleShape(renderWindow, shape, NULL);

            default:break;
            }
        }
    }
}

sfVector2i Checkers_checkedPosition;

enum CheckersState Checkers_lastCheck;

int Checkers_isInField(sfVector2i pos) {
    return pos.x >= 0 && pos.y >= 0 &&
            pos.x < FIELD_WIDTH && pos.y < FIELD_HEIGHT;
}

sfVector2i Checkers_move(sfVector2i position, sfVector2i offset) {
    return (sfVector2i) { .x = position.x + offset.x, .y = position.y + offset.y };
}

/** Multiply field position with some coefficient
 * @param position Field position
 * @param coefficient Coefficient
 * */
sfVector2i Checkers_scale(sfVector2i position, sfVector2i coefficient) {
    return (sfVector2i) { .x = position.x * coefficient.x, .y = position.y * coefficient.y };
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
        offset++;

        // Calculate new point using offset
        nextAvailablePoint = Checkers_move(position,
                Checkers_scale((sfVector2i) { .x = offset, .y = offset }, coefficient));

        if ((Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].state == black ||
                Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].state == white) && !wasChecker)
            wasChecker = 1;

        // Continue loop if point belongs to field & point state is none
        if (Checkers_isInField(nextAvailablePoint) &&
                Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].state == none)
            Checkers_field[nextAvailablePoint.x][nextAvailablePoint.y].state = available;
        else
            break;
    }
}

/** Make available cross with center in position (for big checkers)
 * @param position Field position
 * */
void Checkers_makeAvailableCross(sfVector2i position) {
    // Left - Bottom part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) { .x = 1, .y = 1 });

    // Left - Top part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) { .x = 1, .y = -1 });

    // Right - Bottom part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) { .x = -1, .y = 1 });

    // Right - Top part of cross
    Checkers_makeAvailableLoop(position, (sfVector2i) { .x = -1, .y = -1 });
}

/** Make available next two parts (for small checkers)
 * @param position Field position
 * @param type Checker state (White | Black)
 * */
void Checkers_makeAvailableNearest(sfVector2i position, enum CheckersState type) {
    int coefficient;

    if (type == black)
        coefficient = 1;
    else if (type == white)
        coefficient = -1;
    else
        return;

    int offsetX = 1;

    // Make available:
    // Upper checker
    if (Checkers_field[position.x + offsetX][position.y + coefficient].state == none)
        Checkers_field[position.x + offsetX][position.y + coefficient].state = available;
    else if (Checkers_field[position.x + offsetX][position.y + coefficient].state == black ||
            Checkers_field[position.x + offsetX][position.y + coefficient].state == white)
        if (Checkers_field[position.x + 2 * offsetX][position.y + 2 * coefficient].state == none)
            Checkers_field[position.x + 2 * offsetX][position.y + 2 * coefficient].state = available;
    // Down checker
    if (Checkers_field[position.x - offsetX][position.y + coefficient].state == none)
        Checkers_field[position.x - offsetX][position.y + coefficient].state = available;
    else if (Checkers_field[position.x - offsetX][position.y + coefficient].state == black ||
            Checkers_field[position.x - offsetX][position.y + coefficient].state == white)
        if (Checkers_field[position.x - 2 * offsetX][position.y + 2 * coefficient].state == none)
            Checkers_field[position.x - 2 * offsetX][position.y + 2 * coefficient].state = available;
}

/** Remove all eater checkers after turn
 * @param start Start field position
 * @param stop End field position
 * @returns Any checker was eaten
 * */
int Checkers_removeEatenChecker(sfVector2i start, sfVector2i stop, enum CheckersState type) {
    // Checks if field position is belong to field
    if (!(Checkers_isInField(start) && Checkers_isInField(stop)))
        return -1;

    /** Step size */
    sfVector2i coefficient =
            { .x = stop.x - start.x > 0 ? 1 : -1,
                    .y = stop.y - start.y > 0 ? 1 : -1 };

    /** Counter */
    sfVector2i current = start;

    int wasEaten = 0; // Set it to false by default

    while (current.x != stop.x && current.y != stop.y) {
        // Next step
        current.x += coefficient.x;
        current.y += coefficient.y;

        // If at current position there is checker, remove it & set flag to true
        if (Checkers_field[current.x][current.y].state == type) {
            Checkers_field[current.x][current.y].state = none;
            wasEaten = 1;
        }
    }

    return wasEaten;
}

/** Change current check */
void Checkers_react(sfVector2i position) {
    if (Checkers_field[position.x][position.y].state == black ||
            Checkers_field[position.x][position.y].state == white) {
        // Clear old available checkers
        for (int i = 0; i < FIELD_WIDTH; ++i) {
            for (int j = 0; j < FIELD_HEIGHT; ++j) {
                if (Checkers_field[i][j].state == available)
                    Checkers_field[i][j].state = none;
            }
        }

        // Makes click position (in field) current
        Checkers_checkedPosition = position;

        if (Checkers_field[position.x][position.y].isBig) {
            Checkers_makeAvailableCross(position); // Draw cross
            Checkers_makeAvailableNearest(position, black); // Draw small crosses
            Checkers_makeAvailableNearest(position, white); // Draw small crosses
        } else
            Checkers_makeAvailableNearest(position, Checkers_field[position.x][position.y].state); // Draw small crosses
    } else if (Checkers_field[position.x][position.y].state == available &&
            Checkers_lastCheck != Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].state) {
        int wasEaten = 0;

        if (Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].state == white)
            wasEaten = Checkers_removeEatenChecker(position, Checkers_checkedPosition, black);
        else if (Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].state == black)
            wasEaten = Checkers_removeEatenChecker(position, Checkers_checkedPosition, white);

        // Replace available position with black or white checker
        Checkers_field[position.x][position.y] =
                Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y];

        // Make old checked position empty
        Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y].state = none;

        if (!wasEaten)
            Checkers_lastCheck = Checkers_field[position.x][position.y].state; // Change next turn`s state

        // If check if on the edge of field, make it big
        if (Checkers_lastCheck == white && position.y == 0 ||
                Checkers_lastCheck == black && position.y == FIELD_HEIGHT - 1)
            Checkers_field[position.x][position.y].isBig = 1;
    }
}
