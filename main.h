#ifndef _MAINH_
#define _MAINH_

#include "card.h"

#define len(array) (sizeof(array) / sizeof(*(array)))
#define end(array) (&(array)[len(array)])

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

#endif
