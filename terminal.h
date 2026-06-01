#ifndef _TERMINALH_
#define _TERMINALH_

#include "card.h"
#include <ctype.h>

#define isshift(c) (isupper(c) || (c) == ':')

void init_terminal(void);

void exit_terminal(void);

/**
 * @brief erase all text on-screen and reset the cursor to the top-left corner
 */
void clear_screen(void);

/**
 * @brief draw the top row of a playing card
 *
 * @param card pass 0 to draw a card back
 */
void print_card_top(struct Card card, bool selected);

/**
 * @brief draw the bottom row of a playing card
 *
 * @param card pass 0 to draw a card back
 */
void print_card_bottom(struct Card card, bool selected);

void refresh_screen(void);

#endif
