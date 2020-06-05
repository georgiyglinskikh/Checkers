//
// Created by geo on 03.06.2020.
//

#include "Game.h"

/** Window that checkers are drawn */
sfRenderWindow *Game_renderWindow;

/** Checkers + field are drawn using this shape */
sfRectangleShape *rect;

/** Create window + field */
void Game_create() {
    // Build window mode from settings
    sfVideoMode mode = {.width = WIDTH, .height = HEIGHT, .bitsPerPixel = 32};

    // Create window with given params
    Game_renderWindow = sfRenderWindow_create(mode, "Checkers", sfDefaultStyle, NULL);

    // Remove tearing
    sfRenderWindow_setVerticalSyncEnabled(Game_renderWindow, 1);

    // Fill field using default values
    Checkers_create();

    // Create + make available rectangle shape
    rect = sfRectangleShape_create();
}

/** Draw all on screen */
void Game_draw() {
    // Draw field + checkers on screen
    Checkers_draw(Game_renderWindow, rect);
}

/** Change checkers state */
void Game_update() {
    // React to mouse click
    if (sfMouse_isButtonPressed(sfMouseLeft)) {
        // Get mouse cursor position ->
        sfVector2i mousePosition = sfMouse_getPosition(
                (const sfWindow *) Game_renderWindow);

        // Scale it to field coordinates ->
        sfVector2i mouseScaledPosition = Checkers_scaleFromScreen(Game_renderWindow, mousePosition);

        // Update field using it
        Checkers_react(mouseScaledPosition);
    }
}

/** Main game loop */
void Game_run() {
    /** Event for dispatching */
    sfEvent event;

    // Main game loop
    while (sfRenderWindow_isOpen(Game_renderWindow)) {
        // Dispatch events
        while (sfRenderWindow_pollEvent(Game_renderWindow, &event)) {
            // Close window if close button was pressed
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(Game_renderWindow);
        }

        // Clear screen
        sfRenderWindow_clear(Game_renderWindow, sfBlack);

        Game_draw(); // Draw all on screen

        // Display all game objects on screen
        sfRenderWindow_display(Game_renderWindow);

        Game_update(); // Update scene depending on mouse clicks
    }
}

/** Free resources */
void Game_destroy() {}
