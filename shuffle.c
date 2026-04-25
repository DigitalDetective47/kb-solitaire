#include "card.h"
#include "main.h"
#include "shuffle.h"
#include <stdlib.h>
#include <string.h>

unsigned int brand(unsigned int max)
{
  unsigned int rejection_threshold = ((unsigned)RAND_MAX + 1) / max * max;
  unsigned int r;
  do
  {
    r = rand();
  } while (r >= rejection_threshold);
  return r % max;
}

void new_shuffle()
{
  static struct Card *const destinations[52] = {&tableau[0].cards[0], &tableau[1].cards[0], &tableau[1].cards[1], &tableau[2].cards[0], &tableau[2].cards[1], &tableau[2].cards[2], &tableau[3].cards[0], &tableau[3].cards[1], &tableau[3].cards[2], &tableau[3].cards[3], &tableau[4].cards[0], &tableau[4].cards[1], &tableau[4].cards[2], &tableau[4].cards[3], &tableau[4].cards[4], &tableau[5].cards[0], &tableau[5].cards[1], &tableau[5].cards[2], &tableau[5].cards[3], &tableau[5].cards[4], &tableau[5].cards[5], &tableau[6].cards[0], &tableau[6].cards[1], &tableau[6].cards[2], &tableau[6].cards[3], &tableau[6].cards[4], &tableau[6].cards[5], &tableau[6].cards[6], &drawPile.cards[0], &drawPile.cards[1], &drawPile.cards[2], &drawPile.cards[3], &drawPile.cards[4], &drawPile.cards[5], &drawPile.cards[6], &drawPile.cards[7], &drawPile.cards[8], &drawPile.cards[9], &drawPile.cards[10], &drawPile.cards[11], &drawPile.cards[12], &drawPile.cards[13], &drawPile.cards[14], &drawPile.cards[15], &drawPile.cards[16], &drawPile.cards[17], &drawPile.cards[18], &drawPile.cards[19], &drawPile.cards[20], &drawPile.cards[21], &drawPile.cards[22], &drawPile.cards[23]};
  struct Card unshuffled[52] = {{ACE, SPADES}, {2, SPADES}, {3, SPADES}, {4, SPADES}, {5, SPADES}, {6, SPADES}, {7, SPADES}, {8, SPADES}, {9, SPADES}, {10, SPADES}, {JACK, SPADES}, {QUEEN, SPADES}, {KING, SPADES}, {ACE, CLUBS}, {2, CLUBS}, {3, CLUBS}, {4, CLUBS}, {5, CLUBS}, {6, CLUBS}, {7, CLUBS}, {8, CLUBS}, {9, CLUBS}, {10, CLUBS}, {JACK, CLUBS}, {QUEEN, CLUBS}, {KING, CLUBS}, {ACE, HEARTS}, {2, HEARTS}, {3, HEARTS}, {4, HEARTS}, {5, HEARTS}, {6, HEARTS}, {7, HEARTS}, {8, HEARTS}, {9, HEARTS}, {10, HEARTS}, {JACK, HEARTS}, {QUEEN, HEARTS}, {KING, HEARTS}, {ACE, DIAMONDS}, {2, DIAMONDS}, {3, DIAMONDS}, {4, DIAMONDS}, {5, DIAMONDS}, {6, DIAMONDS}, {7, DIAMONDS}, {8, DIAMONDS}, {9, DIAMONDS}, {10, DIAMONDS}, {JACK, DIAMONDS}, {QUEEN, DIAMONDS}, {KING, DIAMONDS}};

  for (size_t destinations_filled = 0; destinations_filled < len(destinations); destinations_filled++)
  {
    unsigned int r = brand(len(destinations) - destinations_filled);
    *destinations[destinations_filled] = unshuffled[r];
    memmove(unshuffled + r, unshuffled + r + 1, (51 - destinations_filled - r) * sizeof(struct Card));
  }

  drawPile.size = 24;
  drawPile.numFlipped = 24;
  for (size_t index = 0; index < len(tableau); index++)
  {
    tableau[index].numFlipped = index;
    tableau[index].size = index + 1;
  }
}