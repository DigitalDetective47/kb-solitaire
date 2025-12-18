#include "card.h"
#include <stdlib.h>
#include <time.h>

struct CardPile {
  Card cards[24];
  unsigned char size;
  unsigned char numFlipped;
};

struct CardPile drawPile;
struct CardPile tableau[7];
Card foundation[4];

int main() {
  srand(time(NULL));
  return 1;
}
