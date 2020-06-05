//
// Created by geo on 03.06.2020.
//

#include "Game.h"

/** Window that checkers are drawn */
sfRenderWindow *Game_renderWindow;

sfRectangleShape *rect;

void Game_create() {
    // Build window mode from settings
    sfVideoMode mode = {.width = WIDTH, .height = HEIGHT, .bitsPerPixel = 32};

    // Create window with given params
    Game_renderWindow = sfRenderWindow_create(mode, "Checkers", sfDefaultStyle, NULL);

    // Remove tearing
    sfRenderWindow_setVerticalSyncEnabled(Game_renderWindow, 1);

    Checkers_create();

    rect = sfRectangleShape_create();
}

void Game_draw();

void Game_update();

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

        Game_draw();

        // Display all game objects on screen
        sfRenderWindow_display(Game_renderWindow);

        Game_update();
    }
}

void Game_draw() {
    Checkers_draw(Game_renderWindow, rect);
}

void Game_update() {
    if (sfMouse_isButtonPressed(sfMouseLeft)) {
        Checkers_react(Checkers_scaleFromScreen(Game_renderWindow, sfMouse_getPosition(
                (const sfWindow *) Game_renderWindow)));
    }
}

void Game_destroy() {

}
