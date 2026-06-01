#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

/**
 * @brief whether the current game is won
 */
bool won(void);

/**
 * @brief handles the main game logic; meant to be called in a loop
 */
void game(void);

/**
 * @brief attempt a move
 */
void try_move(struct Selection from, union Selectable to);

#endif