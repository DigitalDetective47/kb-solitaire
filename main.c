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
struct Card foundation[4];

struct Selection selection;

int main()
{
  srand(time(NULL));
  init_terminal();

  new_shuffle();
  refresh_screen();
  while (!won())
  {
    game();
  }

  return 0;
}

struct Card top(union Selectable target)
{
  if (in(target.card, foundation))
  {
    return *target.card;
  }
  else if (target.card_pile == &drawPile)
  {
    return drawPile.size == drawPile.numFlipped ? NILCARD : drawPile.cards[drawPile.numFlipped];
  }
  else
  {
    return target.card_pile->size ? target.card_pile->cards[target.card_pile->size - 1] : NILCARD;
  }
}

void pop(union Selectable target)
{
  if (in(target.card, foundation))
  {
    const struct Card removed = top(target);
    if (removed.rank == ACE)
    {
      *target.card = NILCARD;
    }
    else
    {
      *target.card = (struct Card){removed.rank - 1, removed.suit};
    }
  }
  else if (target.card_pile == &drawPile)
  {
    memmove(&drawPile.cards[drawPile.numFlipped], &drawPile.cards[drawPile.numFlipped + 1], (drawPile.size - drawPile.numFlipped - 1) * sizeof(struct Card));
    drawPile.size--;
  }
  else
  {
    target.card_pile->size--;
    if (target.card_pile->numFlipped && target.card_pile->size == target.card_pile->numFlipped)
    {
      target.card_pile->numFlipped--;
    }
  }
}

void push(union Selectable target, struct Card card)
{
  if (in(target.card, foundation))
  {
    *target.card = card;
  }
  else
  {
    target.card_pile->cards[target.card_pile->size] = card;
    target.card_pile->size++;
  }
}
