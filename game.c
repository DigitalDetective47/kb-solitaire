#include "game.h"
#include "card.h"
#include "main.h"
#include "terminal.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

inline bool won() {
  for (Card const *f = &foundation[0]; f < end(foundation); f++) {
    if (RANK(*f) != KING) {
      return false;
    }
  }
  return true;
}

inline void game() {
  char input;
  if (scanf("%c", &input) != 1) {
    return; // some error reading input
  }
  union Selectable dest;
  switch (input) {
  case 'q':
  case 'Q':
    if (drawPile.numFlipped) {
      drawPile.numFlipped--;
    } else {
      drawPile.numFlipped = drawPile.size;
    }
    [[fallthrough]];
  case 'e':
  case 'E':
    selection.ptr.card = NULL;
    refresh_screen();
    return;
  case 'w':
  case 'W':
    selection.ptr.card_pile =
        selection.ptr.card_pile == &drawPile ? NULL : &drawPile;
    selection.size = 1;
    refresh_screen();
    return;
  case 'u':
  case 'U':
    dest.card = &foundation[0];
    break;
  case 'i':
  case 'I':
    dest.card = &foundation[1];
    break;
  case 'o':
  case 'O':
    dest.card = &foundation[2];
    break;
  case 'p':
  case 'P':
    dest.card = &foundation[3];
    break;
  case 'a':
  case 'A':
    dest.card_pile = &tableau[0];
    break;
  case 's':
  case 'S':
    dest.card_pile = &tableau[1];
    break;
  case 'd':
  case 'D':
    dest.card_pile = &tableau[2];
    break;
  case 'j':
  case 'J':
    dest.card_pile = &tableau[3];
    break;
  case 'k':
  case 'K':
    dest.card_pile = &tableau[4];
    break;
  case 'l':
  case 'L':
    dest.card_pile = &tableau[5];
    break;
  case ';':
  case ':':
    dest.card_pile = &tableau[6];
    break;
  default:
    return;
  }

  if (selection.ptr.card) {
    if (selection.ptr.card == dest.card) {
      if (in(dest.card, foundation)) {
        selection.ptr.card = NULL;
      } else if (isupper(input) || input == ':') {
        if (selection.size == 1) {
          selection.size = selection.ptr.card_pile->size -
                           selection.ptr.card_pile->numFlipped;
        } else {
          selection.size--;
        }
      } else {
        if (selection.size == selection.ptr.card_pile->size -
                                  selection.ptr.card_pile->numFlipped) {
          selection.size = 1;
        } else {
          selection.size++;
        }
      }
    } else {
      try_move(selection, dest);
      selection.ptr.card_pile = NULL;
    }
  } else if (top(dest)) {
    selection.ptr = dest;
    selection.size =
        in(dest.card_pile, tableau) && (isupper(input) || input == ':')
            ? selection.ptr.card_pile->size -
                  selection.ptr.card_pile->numFlipped
            : 1;
  } else {
    return;
  }

  refresh_screen();
}

inline void try_move(struct Selection from, union Selectable to) {
  if (in(from.ptr.card_pile, tableau) && in(to.card_pile, tableau)) {
    const Card move_root =
        from.ptr.card_pile->cards[from.ptr.card_pile->size - from.size];
    const Card recipient = top(to);
    if ((!recipient && RANK(move_root) == KING) ||
        (RANK(move_root) + 1 == RANK(recipient) &&
         COLOR(move_root) != COLOR(recipient))) {
      from.ptr.card_pile->size -= from.size;
      if (from.ptr.card_pile->numFlipped &&
          from.ptr.card_pile->size == from.ptr.card_pile->numFlipped) {
        from.ptr.card_pile->numFlipped--;
      }
      memcpy(&to.card_pile->cards[to.card_pile->size],
             &from.ptr.card_pile->cards[from.ptr.card_pile->size], from.size);
      to.card_pile->size += from.size;
    }
  } else if (from.size != 1) {
    return;
  } else if (in(to.card, foundation)) {
    const Card moving = top(from.ptr);
    const Card recipient = *to.card;
    if ((!recipient && RANK(moving) == ACE) || moving == recipient + 1) {
      pop(from.ptr);
      push(to, moving);
    }
  } else {
    const Card moving = top(from.ptr);
    const Card recipient = top(to);
    if ((!recipient && RANK(moving) == KING) ||
        (RANK(moving) + 1 == RANK(recipient) &&
         COLOR(moving) != COLOR(recipient))) {
      pop(from.ptr);
      push(to, moving);
    }
  }
}
