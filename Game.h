//
// Created by geo on 03.06.2020.
//

#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include "Checkers.h"

/** Window width */
#define WIDTH 600

/** Window height */
#define HEIGHT 600

/** Init window + create checkers */
void Game_create();

/** Start main game loop */
void Game_run();

/** Free game resources after finish */
void Game_destroy();

#endif //CHECKERS_GAME_H
