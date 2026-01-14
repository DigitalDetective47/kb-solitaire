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
  scanf("%c", &input);
  void *dest;
  switch (input) {
  case 'q':
  case 'Q':
    if (drawPile.numFlipped) {
      drawPile.numFlipped--;
    } else {
      drawPile.numFlipped = drawPile.size;
    }
  case 'e':
  case 'E':
    selection.ptr = NULL;
    refresh_screen();
    return;
  case 'w':
  case 'W':
    selection.ptr = selection.ptr == &drawPile ? NULL : &drawPile;
    selection.size = 1;
    refresh_screen();
    return;
  case 'u':
  case 'U':
    dest = &foundation[0];
    break;
  case 'i':
  case 'I':
    dest = &foundation[1];
    break;
  case 'o':
  case 'O':
    dest = &foundation[2];
    break;
  case 'p':
  case 'P':
    dest = &foundation[3];
    break;
  case 'a':
  case 'A':
    dest = &tableau[0];
    break;
  case 's':
  case 'S':
    dest = &tableau[1];
    break;
  case 'd':
  case 'D':
    dest = &tableau[2];
    break;
  case 'j':
  case 'J':
    dest = &tableau[3];
    break;
  case 'k':
  case 'K':
    dest = &tableau[4];
    break;
  case 'l':
  case 'L':
    dest = &tableau[5];
    break;
  case ';':
  case ':':
    dest = &tableau[6];
    break;
  default:
    return;
  }

  if (selection.ptr) {
    if (selection.ptr == dest) {
      if (in(dest, foundation)) {
        selection.ptr = NULL;
      } else if (isupper(input) || input == ':') {
        if (selection.size == 1) {
          selection.size = ((struct CardPile *)selection.ptr)->size -
                           ((struct CardPile *)selection.ptr)->numFlipped;
        } else {
          selection.size--;
        }
      } else {
        if (selection.size ==
            ((struct CardPile *)selection.ptr)->size -
                ((struct CardPile *)selection.ptr)->numFlipped) {
          selection.size = 1;
        } else {
          selection.size++;
        }
      }
    } else {
      try_move(selection, dest);
      selection.ptr = NULL;
    }
  } else if (top(dest)) {
    selection.ptr = dest;
    selection.size = in(dest, tableau) && (isupper(input) || input == ':')
                         ? ((struct CardPile *)selection.ptr)->size -
                               ((struct CardPile *)selection.ptr)->numFlipped
                         : 1;
  } else {
    return;
  }

  refresh_screen();
}

inline void try_move(struct Selection from, void *to) {
  if (in(from.ptr, tableau) && in(to, tableau)) {
    const Card move_root =
        ((struct CardPile *)from.ptr)
            ->cards[((struct CardPile *)from.ptr)->size - from.size];
    const Card recipient = top(to);
    if ((!recipient && RANK(move_root) == KING) ||
        (RANK(move_root) + 1 == RANK(recipient) &&
         COLOR(move_root) != COLOR(recipient))) {
      ((struct CardPile *)from.ptr)->size -= from.size;
      if (((struct CardPile *)from.ptr)->numFlipped &&
          ((struct CardPile *)from.ptr)->size ==
              ((struct CardPile *)from.ptr)->numFlipped) {
        ((struct CardPile *)from.ptr)->numFlipped--;
      }
      memcpy(&((struct CardPile *)to)->cards[((struct CardPile *)to)->size],
             &((struct CardPile *)from.ptr)
                  ->cards[((struct CardPile *)from.ptr)->size],
             from.size);
      ((struct CardPile *)to)->size += from.size;
    }
  } else if (from.size != 1) {
    return;
  } else if (in(to, foundation)) {
    const Card moving = top(from.ptr);
    const Card recipient = *(Card *)to;
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
