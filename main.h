#ifndef _MAINH_
#define _MAINH_

#include "card.h"

#define len(array) (sizeof(array) / sizeof(*(array)))
#define end(array) (&(array)[len(array)])
#define in(ptr, array) ((ptr) >= (array) && (ptr) < end(array))

struct CardPile
{
  struct Card cards[24];
  size_t size;
  size_t numFlipped;
};

extern struct CardPile drawPile;
extern struct CardPile tableau[7];
extern struct Card foundation[4];

union Selectable
{
  struct Card *card;
  struct CardPile *card_pile;
};

struct Selection
{
  union Selectable ptr;
  size_t size;
};

extern struct Selection selection;

int main();

/**
 * @brief get the top card of a selection area
 */
struct Card top(union Selectable target);

/**
 * @brief remove the top card from an area
 * undefined behavior if the target area is empty
 */
void pop(union Selectable target);

/**
 * @brief add a card to an area
 */
void push(union Selectable target, struct Card card);

#endif
