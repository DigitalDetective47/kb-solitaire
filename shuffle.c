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
  static Card *const destinations[52] = {
      &tableau[0].cards[0], &tableau[1].cards[0], &tableau[1].cards[1],
      &tableau[2].cards[0], &tableau[2].cards[1], &tableau[2].cards[2],
      &tableau[3].cards[0], &tableau[3].cards[1], &tableau[3].cards[2],
      &tableau[3].cards[3], &tableau[4].cards[0], &tableau[4].cards[1],
      &tableau[4].cards[2], &tableau[4].cards[3], &tableau[4].cards[4],
      &tableau[5].cards[0], &tableau[5].cards[1], &tableau[5].cards[2],
      &tableau[5].cards[3], &tableau[5].cards[4], &tableau[5].cards[5],
      &tableau[6].cards[0], &tableau[6].cards[1], &tableau[6].cards[2],
      &tableau[6].cards[3], &tableau[6].cards[4], &tableau[6].cards[5],
      &tableau[6].cards[6], &drawPile.cards[0],   &drawPile.cards[1],
      &drawPile.cards[2],   &drawPile.cards[3],   &drawPile.cards[4],
      &drawPile.cards[5],   &drawPile.cards[6],   &drawPile.cards[7],
      &drawPile.cards[8],   &drawPile.cards[9],   &drawPile.cards[10],
      &drawPile.cards[11],  &drawPile.cards[12],  &drawPile.cards[13],
      &drawPile.cards[14],  &drawPile.cards[15],  &drawPile.cards[16],
      &drawPile.cards[17],  &drawPile.cards[18],  &drawPile.cards[19],
      &drawPile.cards[20],  &drawPile.cards[21],  &drawPile.cards[22],
      &drawPile.cards[23],
  };
  Card unshuffled[52] = {
      CARD(ACE, SPADES),    CARD(2, SPADES),      CARD(3, SPADES),
      CARD(4, SPADES),      CARD(5, SPADES),      CARD(6, SPADES),
      CARD(7, SPADES),      CARD(8, SPADES),      CARD(9, SPADES),
      CARD(10, SPADES),     CARD(JACK, SPADES),   CARD(QUEEN, SPADES),
      CARD(KING, SPADES),   CARD(ACE, CLUBS),     CARD(2, CLUBS),
      CARD(3, CLUBS),       CARD(4, CLUBS),       CARD(5, CLUBS),
      CARD(6, CLUBS),       CARD(7, CLUBS),       CARD(8, CLUBS),
      CARD(9, CLUBS),       CARD(10, CLUBS),      CARD(JACK, CLUBS),
      CARD(QUEEN, CLUBS),   CARD(KING, CLUBS),    CARD(ACE, HEARTS),
      CARD(2, HEARTS),      CARD(3, HEARTS),      CARD(4, HEARTS),
      CARD(5, HEARTS),      CARD(6, HEARTS),      CARD(7, HEARTS),
      CARD(8, HEARTS),      CARD(9, HEARTS),      CARD(10, HEARTS),
      CARD(JACK, HEARTS),   CARD(QUEEN, HEARTS),  CARD(KING, HEARTS),
      CARD(ACE, DIAMONDS),  CARD(2, DIAMONDS),    CARD(3, DIAMONDS),
      CARD(4, DIAMONDS),    CARD(5, DIAMONDS),    CARD(6, DIAMONDS),
      CARD(7, DIAMONDS),    CARD(8, DIAMONDS),    CARD(9, DIAMONDS),
      CARD(10, DIAMONDS),   CARD(JACK, DIAMONDS), CARD(QUEEN, DIAMONDS),
      CARD(KING, DIAMONDS),
  };

  for (unsigned char destinations_filled = 0; destinations_filled < 52;
       destinations_filled++) {
    unsigned int r = brand(52 - destinations_filled);
    *destinations[destinations_filled] = unshuffled[r];
    memmove(unshuffled + r, unshuffled + r + 1, 51 - destinations_filled - r);
  }

  drawPile.size = 24;
  drawPile.numFlipped = 24;
  for (unsigned char index = 0; index < len(tableau); index++) {
    tableau[index].numFlipped = index;
    tableau[index].size = index + 1;
  }
}