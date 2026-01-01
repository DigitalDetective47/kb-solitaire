#ifndef _TERMINALH_
#define _TERMINALH_

#include "card.h"

/**
 * @brief erase all text on-screen and reset the cursor to the top-left corner
 */
void clear_screen();

/**
 * @brief draw the top row of a playing card
 *
 * @param card pass 0 to draw a card back
 */
void print_card_top(Card card, bool selected, bool underline);

/**
 * @brief draw the bottom row of a playing card
 *
 * @param card pass 0 to draw a card back
 */
void print_card_bottom(Card card, bool selected);

void refresh_screen();

#endif
