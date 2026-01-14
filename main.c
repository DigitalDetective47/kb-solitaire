#include "main.h"
#include "card.h"
#include "game.h"
#include "shuffle.h"
#include "terminal.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct CardPile drawPile;
struct CardPile tableau[7];
Card foundation[4];

struct Selection selection;

int main() {
  srand(time(NULL));
  init_terminal();

  new_shuffle();
  refresh_screen();
  while (!won()) {
    game();
  }

  return 0;
}

Card top(void const *target) {
  if (in(target, foundation)) {
    return *(Card const *)target;
  } else if (target == &drawPile) {
    return drawPile.size == drawPile.numFlipped
               ? 0
               : drawPile.cards[drawPile.numFlipped];
  } else {
    return ((struct CardPile const *)target)->size
               ? ((struct CardPile const *)target)
                     ->cards[((struct CardPile const *)target)->size - 1]
               : 0;
  }
}

inline void pop(void *target) {
  if (in(target, foundation)) {
    const Card removed = top(target);
    if (RANK(removed) == ACE) {
      *(Card *)target = 0;
    } else {
      *(Card *)target = removed - 1;
    }
  } else if (target == &drawPile) {
    memmove(&drawPile.cards[drawPile.numFlipped],
            &drawPile.cards[drawPile.numFlipped + 1],
            drawPile.size - drawPile.numFlipped - 1);
    drawPile.size--;
  } else {
    ((struct CardPile *)target)->size--;
    if (((struct CardPile *)target)->numFlipped &&
        ((struct CardPile *)target)->size ==
            ((struct CardPile *)target)->numFlipped) {
      ((struct CardPile *)target)->numFlipped--;
    }
  }
}

inline void push(void *target, Card card) {
  if (in(target, foundation)) {
    *(Card *)target = card;
  } else{
    ((struct CardPile *)target)->cards[((struct CardPile *)target)->size] = card;
    ((struct CardPile *)target)->size++;
  }
}
