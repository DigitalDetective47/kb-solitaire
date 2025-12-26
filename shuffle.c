#include <stdlib.h>
#include <string.h>

#include "card.h"
#include "main.h"
#include "shuffle.h"

inline unsigned int brand(unsigned int max) {
  unsigned int rejection_threshold = ((unsigned)RAND_MAX + 1) / max * max;
  unsigned int r;
  do {
    r = rand();
  } while (r >= rejection_threshold);
  return r % max;
}

void new_shuffle() {
  Card *destinations[52];
  unsigned char destinations_filled = 0;
  for (Card *ptr = drawPile.cards; ptr < drawPile.cards + 24; ptr++) {
    destinations[destinations_filled++] = ptr;
  }
  for (unsigned char tableau_index = 0;
       tableau_index < sizeof(tableau) / sizeof(*tableau); tableau_index++) {
    struct CardPile *current_pile = tableau + tableau_index;
    current_pile->numFlipped = tableau_index;
    current_pile->size = tableau_index + 1;
    for (Card *ptr = current_pile->cards;
         ptr < current_pile->cards + tableau_index + 1; ptr++) {
      destinations[destinations_filled++] = ptr;
    }
  }

  Card unshuffled[52];
  destinations_filled = 0;
  for (enum Suit suit = SPADES; suit <= DIAMONDS; suit++) {
    for (unsigned char rank = ACE; rank <= KING; rank++) {
      unshuffled[destinations_filled++] = CARD(rank, suit);
    }
  }

  for (destinations_filled = 0; destinations_filled < 52;
       destinations_filled++) {
    unsigned int r = brand(51 - destinations_filled);
    *destinations[destinations_filled] = unshuffled[r];
    memmove(unshuffled + r, unshuffled + r + 1, 51 - destinations_filled - r);
  }
}