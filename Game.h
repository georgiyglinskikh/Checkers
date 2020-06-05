//
// Created by geo on 03.06.2020.
//

#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include "Checkers.h"

// region Settings

/** Window width */
#define WIDTH 600

/** Window height */
#define HEIGHT 600

// endregion

// region Methods

/** Init window + create checkers */
void Game_create();

/** Start main game loop */
void Game_run();

/** Free game resources after finish */
void Game_destroy();

// endregion

#endif //CHECKERS_GAME_H
