#ifndef _TERMINALH_
#define _TERMINALH_

#include "card.h"

void init_terminal();

void exit_terminal();

/**
 * @brief erase all text on-screen and reset the cursor to the top-left corner
 */
void clear_screen();

/**
 * @brief draw the top row of a playing card
 *
 * @param card pass 0 to draw a card back
 */
void print_card_top(Card card, bool selected);

/**
 * @brief draw the bottom row of a playing card
 *
 * @param card pass 0 to draw a card back
 */
void print_card_bottom(Card card, bool selected);

void refresh_screen();

/**
 * @brief get a valid move key
 * blocks until an action is available
 */
char get_action();

#endif
