#ifndef _CARDH_
#define _CARDH_

typedef unsigned char Card;

#define ACE 1
#define JACK 11
#define QUEEN 12
#define KING 13

enum Suit {
  SPADES = 0,
  CLUBS = 1,
  HEARTS = 2,
  DIAMONDS = 3,
};

enum Color {
  BLACK = 0,
  RED = 1,
};

#define RANK(c) ((c) & 15)
#define SUIT(c) ((c) >> 4)
#define COLOR(c) ((c) >> 5)

#define CARD(rank, suit) ((rank) + ((suit) << 4))

#endif
