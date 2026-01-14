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

Card top(union Selectable target) {
  if (in(target.card, foundation)) {
    return *target.card;
  } else if (target.card_pile == &drawPile) {
    return drawPile.size == drawPile.numFlipped
               ? 0
               : drawPile.cards[drawPile.numFlipped];
  } else {
    return target.card_pile->size
               ? target.card_pile->cards[target.card_pile->size - 1]
               : 0;
  }
}

inline void pop(union Selectable target) {
  if (in(target.card, foundation)) {
    const Card removed = top(target);
    if (RANK(removed) == ACE) {
      *target.card = 0;
    } else {
      *target.card = removed - 1;
    }
  } else if (target.card_pile == &drawPile) {
    memmove(&drawPile.cards[drawPile.numFlipped],
            &drawPile.cards[drawPile.numFlipped + 1],
            drawPile.size - drawPile.numFlipped - 1);
    drawPile.size--;
  } else {
    target.card_pile->size--;
    if (target.card_pile->numFlipped &&
        target.card_pile->size == target.card_pile->numFlipped) {
      target.card_pile->numFlipped--;
    }
  }
}

inline void push(union Selectable target, Card card) {
  if (in(target.card, foundation)) {
    *target.card = card;
  } else {
    target.card_pile->cards[target.card_pile->size] = card;
    target.card_pile->size++;
  }
}
