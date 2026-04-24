#ifndef _CARDH_
#define _CARDH_

#define ACE 1
#define JACK 11
#define QUEEN 12
#define KING 13

enum Suit
{
  SPADES = 0,
  CLUBS = 1,
  HEARTS = 2,
  DIAMONDS = 3,
};

enum Color
{
  BLACK = 0,
  RED = 1,
};

#define COLOR(s) ((s) >> 1)

struct Card
{
  unsigned rank : 4;
  enum Suit suit : 2;
};

#define CARDEXISTS(c) ((c).rank != 0)
#define NILCARD (struct Card){0, 0}

#endif
