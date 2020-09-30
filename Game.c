#include "Game.h"

sfRenderWindow* Game_renderWindow;
sfRectangleShape* rect;

void Game_create() {
    sfVideoMode mode = { .width = WIDTH, .height = HEIGHT, .bitsPerPixel = 32 };

    Game_renderWindow = sfRenderWindow_create(mode, "Checkers", sfDefaultStyle, NULL);
    sfRenderWindow_setVerticalSyncEnabled(Game_renderWindow, 1); // Remove tearing

    rect = sfRectangleShape_create();

    Checkers_create();

}

void Game_draw() {
    Checkers_draw(Game_renderWindow, rect);
}

void Game_update() {
    if (sfMouse_isButtonPressed(sfMouseLeft)) {
        sfVector2i mousePosition = sfMouse_getPosition(
                (const sfWindow*) Game_renderWindow);
        sfVector2i mouseScaledPosition = Checkers_scaleFromScreen(Game_renderWindow, mousePosition);

        Checkers_react(mouseScaledPosition);
    }
}

void Game_run() {
    sfEvent event;
    while (sfRenderWindow_isOpen(Game_renderWindow)) {
        // Dispatch events
        while (sfRenderWindow_pollEvent(Game_renderWindow, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(Game_renderWindow);
        }

        sfRenderWindow_clear(Game_renderWindow, sfBlack);
        Game_draw();
        sfRenderWindow_display(Game_renderWindow);

        Game_update(); // Update scene depending on mouse clicks
    }
}

void Game_destroy() {
    sfRenderWindow_destroy(Game_renderWindow);
}
