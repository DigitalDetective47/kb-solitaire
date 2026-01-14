#ifndef _MAINH_
#define _MAINH_

#include "card.h"

#define len(array) (sizeof(array) / sizeof(*(array)))
#define end(array) (&(array)[len(array)])
#define in(ptr, array) ((ptr) >= (array) && (ptr) < end(array))

struct CardPile {
  Card cards[24];
  unsigned char size;
  unsigned char numFlipped;
};

extern struct CardPile drawPile;
extern struct CardPile tableau[7];
extern Card foundation[4];

union Selectable {
  Card *card;
  struct CardPile *card_pile;
};

struct Selection {
  union Selectable ptr;
  unsigned char size;
};

extern struct Selection selection;

int main();

/**
 * @brief get the top card of a selection area
 */
Card top(union Selectable target);

/**
 * @brief remove the top card from an area
 * undefined behavior if the target area is empty
 */
void pop(union Selectable target);

/**
 * @brief add a card to an area
 */
void push(union Selectable target, Card card);

#endif
