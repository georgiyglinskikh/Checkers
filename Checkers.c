//
// Created by geo on 03.06.2020.
//

#include "Checkers.h"

/** Field containing all checkers */
enum CheckersType Checkers_field[FIELD_WIDTH][FIELD_HEIGHT];

/** Create all for drawing */
void Checkers_create() {
    // Generate field
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            if ((i + j) % 2) {
                if (j < 3) // Set black checkers at top
                    Checkers_field[i][j] = black;
                else if (j >= FIELD_HEIGHT - 3) // Set white checkers at bottom
                    Checkers_field[i][j] = white;
                else // All other positions are empty
                    Checkers_field[i][j] = none;
            }
        }
    }
}

/** Print field to console as chars */
void Checkers_log() {
    // Print checkers
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            // Print checker as char
            printf("%c ", Checkers_field[j][i]);
        }
        // Return + new line
        printf("\n");
    }
}

/** Scale vector to draw on screen */
sfVector2f Checkers_scaleToScreen(sfRenderWindow *renderWindow, sfVector2i pos) {
    // Save window size to save resources
    sfVector2u win_size = sfRenderWindow_getSize(renderWindow);

    /* Scaled vector = {
     *      x' = x / width * screen width
     *      y' = y / height * screen height
     * } -- It convert field positions to draw on screen */
    return (sfVector2f) {.x = (float) pos.x / (float) FIELD_WIDTH * (float) win_size.x,
            .y = (float) pos.y / (float) FIELD_HEIGHT * (float) win_size.y};
}

/** Scale vector to get clicks from screen */
sfVector2i Checkers_scaleFromScreen(sfRenderWindow *renderWindow, sfVector2i vec) {
    // Save window size to save resources
    sfVector2u win_size = sfRenderWindow_getSize(renderWindow);

    /* Scaled vector = {
     *      x' = x / screen width * width
     *      y' = y / screen height *  height
     * } -- It convert click position to field one */
    return (sfVector2i) {.x = (int) ((float) vec.x / (float) win_size.x * FIELD_WIDTH),
            .y = (int) ((float) vec.y / (float) win_size.y * FIELD_HEIGHT)};
}

/** Draw field on given window */
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
            switch (Checkers_field[i][j]) {
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

/** Change current check */
void Checkers_react(sfVector2i position) {
    if (Checkers_field[position.x][position.y] == black) {
        Checkers_checkedPosition = position;

        if (Checkers_field[position.x + 1][position.y + 1] == none)
            Checkers_field[position.x + 1][position.y + 1] = available;

        if (Checkers_field[position.x - 1][position.y + 1] == none)
            Checkers_field[position.x - 1][position.y + 1] = available;
    } else if (Checkers_field[position.x][position.y] == white) {
        Checkers_checkedPosition = position;

        if (Checkers_field[position.x + 1][position.y - 1] == none)
            Checkers_field[position.x + 1][position.y - 1] = available;

        if (Checkers_field[position.x - 1][position.y - 1] == none) {
            Checkers_field[position.x - 1][position.y - 1] = available;
        }
    } else if (Checkers_field[position.x][position.y] == available) {
        Checkers_field[position.x][position.y] =
                Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y];

        Checkers_field[Checkers_checkedPosition.x][Checkers_checkedPosition.y] = none;

        for (int i = 0; i < FIELD_WIDTH; ++i) {
            for (int j = 0; j < FIELD_HEIGHT; ++j) {
                if (Checkers_field[i][j] == available)
                    Checkers_field[i][j] = none;
            }
        }
    }
}
