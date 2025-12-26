#ifndef _MAINH_
#define _MAINH_

#include "card.h"
#include "terminal.h"

struct CardPile {
  Card cards[24];
  unsigned char size;
  unsigned char numFlipped;
};

extern struct CardPile drawPile;
extern struct CardPile tableau[7];
extern Card foundation[4];

int main();

#endif
