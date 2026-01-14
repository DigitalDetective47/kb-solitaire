#ifndef _MAINH_
#define _MAINH_

#include "card.h"

#define len(array) (sizeof(array) / sizeof(*(array)))
#define end(array) (&(array)[len(array)])
#define in(ptr, array) ((ptr) >= (const void *)(array) && (ptr) < (const void *)end(array))

struct CardPile {
  Card cards[24];
  unsigned char size;
  unsigned char numFlipped;
};

extern struct CardPile drawPile;
extern struct CardPile tableau[7];
extern Card foundation[4];

struct Selection {
  void *ptr;
  unsigned char size;
};

extern struct Selection selection;

int main();

/**
 * @brief get the top card of a selection area
 */
Card top(void const *target);

/**
 * @brief remove the top card from an area
 * undefined behavior if the target area is empty
 */
void pop(void *target);

/**
 * @brief add a card to an area
 */
void push(void *target, Card card);

#endif
