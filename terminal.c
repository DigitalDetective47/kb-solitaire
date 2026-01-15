#include "terminal.h"
#include "card.h"
#include "main.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios outer_settings;

inline void init_terminal() {
  tcgetattr(STDIN_FILENO, &outer_settings);
  struct termios settings = outer_settings;
  settings.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &settings);
  atexit(exit_terminal);
}
void exit_terminal() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &outer_settings); }

inline void clear_screen() { printf("\e[2J\e[3J\e[;H"); }

void setup_card_colors(Card card, bool selected) {
  printf(mFGCOLOR);
  switch (COLOR(card)) {
  case BLACK:
    printf(mBLACK);
    break;
  case RED:
    printf(mRED);
    break;
  }
  printf(";" mBGCOLOR);
  printf(selected ? mGREEN : mWHITE);
  printf("m");
}

void print_card_top(Card card, bool selected) {
  printf("\e[");
  if (card) {
    setup_card_colors(card, selected);
    static const char rank_chars[] = {'A', '2', '3', '4', '5', '6', '7',
                                      '8', '9', '1', 'J', 'Q', 'K'};
    printf("%c", rank_chars[RANK(card) - 1]);
    static const char suit_chars[4][4] = {uSPADES, uCLUBS, uHEARTS, uDIAMONDS};
    printf("%s", suit_chars[SUIT(card)]);
  } else {
    printf(mFGCOLOR mWHITE ";" mBGCOLOR mRED "m" uSPADES uHEARTS);
  }
  printf("\e[m");
}

void print_card_bottom(Card card, bool selected) {
  printf("\e[");
  if (card) {
    setup_card_colors(card, selected);
    printf(RANK(card) == 10 ? "0 " : "  ");
  } else {
    printf(mFGCOLOR mWHITE ";" mBGCOLOR mRED "m" uDIAMONDS uCLUBS);
  }
  printf("\e[m");
}

void refresh_screen() {
  clear_screen();

  // row 1
  printf(" Q ");
  printf(selection.ptr.card_pile == &drawPile ? "\e[32m" uTOPLEFT
                                                "\e[mW \e[32m" uTOPRIGHT "\e[m"
                                              : " W  ");
  printf("E ");
  for (unsigned char index = 0; index <= len(foundation); index++) {
    static const char keys[4] = {'U', 'I', 'O', 'P'};
    if (index && selection.ptr.card == &foundation[index - 1]) {
      printf("\e[" mFGCOLOR mGREEN "m" uTOPRIGHT "\e[m");
    } else if (index < len(foundation) &&
               selection.ptr.card == &foundation[index]) {
      printf("\e[" mFGCOLOR mGREEN "m" uTOPLEFT "\e[m");
    } else {
      printf(" ");
    }
    if (index < len(foundation)) {
      printf("%c ", keys[index]);
    }
  }

  // row 2
  printf("\n ");
  if (drawPile.numFlipped) {
    print_card_top(0, false);
  } else {
    printf("  ");
  }
  printf(" ");
  if (drawPile.numFlipped < drawPile.size) {
    print_card_top(top((union Selectable){.card_pile = &drawPile}),
                   selection.ptr.card_pile == &drawPile);
  } else {
    printf("  ");
  }
  printf(" \e[" mFGCOLOR mRED "m" uBSLASH uFSLASH "\e[m ");
  for (Card const *f = &foundation[0]; f < end(foundation); f++) {
    if (*f) {
      print_card_top(*f, selection.ptr.card == f);
    } else {
      printf("  ");
    }
    printf(" ");
  }

  // row 3
  printf("\n ");
  if (drawPile.numFlipped) {
    print_card_bottom(0, false);
  } else {
    printf("  ");
  }
  printf(" ");
  if (drawPile.numFlipped < drawPile.size) {
    print_card_bottom(top((union Selectable){.card_pile = &drawPile}),
                      selection.ptr.card_pile == &drawPile);
  } else {
    printf("  ");
  }
  printf(" \e[" mFGCOLOR mRED "m" uFSLASH uBSLASH "\e[m ");
  for (Card const *f = &foundation[0]; f < end(foundation); f++) {
    if (*f) {
      print_card_bottom(*f, selection.ptr.card == f);
    } else {
      printf("  ");
    }
    printf(" ");
  }

  // row 4
  printf("\n");
  if (selection.ptr.card) {
    static const union Selectable upper_selections[9] = {
        {.card = NULL},
        {.card = NULL},
        {.card_pile = &drawPile},
        {.card = NULL},
        {.card = &foundation[0]},
        {.card = &foundation[1]},
        {.card = &foundation[2]},
        {.card = &foundation[3]},
        {.card = NULL}};
    for (unsigned char i = 0; i <= 7; i++) {
      if (selection.ptr.card == upper_selections[i].card) {
        printf("\e[" mFGCOLOR mGREEN "m" uBOTTOMRIGHT "\e[m");
      } else if (selection.ptr.card == upper_selections[i + 1].card) {
        printf("\e[" mFGCOLOR mGREEN "m" uBOTTOMLEFT "\e[m");
      } else if (i && selection.ptr.card_pile == &tableau[i - 1] &&
                 selection.size == tableau[i - 1].size) {
        printf("\e[" mFGCOLOR mGREEN "m" uTOPRIGHT "\e[m");
      } else if (i < 7 && selection.ptr.card_pile == &tableau[i] &&
                 selection.size == tableau[i].size) {
        printf("\e[" mFGCOLOR mGREEN "m" uTOPLEFT "\e[m");
      } else {
        printf(" ");
      }
      if (i < 7) {
        static const char keys[7] = {'A', 'S', 'D', 'J', 'K', 'L', ';'};
        printf("%c ", keys[i]);
      }
    }
  } else {
    printf(" A  S  D  J  K  L  ;  ");
  }

  bool again = true;
  for (unsigned char row = 0; again; row++) {
    again = false;
    printf("\n");
    for (unsigned char column = 0; column <= len(tableau); column++) {
      if (column && selection.ptr.card_pile == &tableau[column - 1]) {
        if (tableau[column - 1].size + 1 == row) {
          printf("\e[" mFGCOLOR mGREEN "m" uBOTTOMRIGHT "\e[m");
        } else if (tableau[column - 1].size - selection.size == row + 1) {
          printf("\e[" mFGCOLOR mGREEN "m" uTOPRIGHT "\e[m");
        } else {
          printf(" ");
        }
      } else if (column < len(tableau)) {
        if (selection.ptr.card_pile == &tableau[column]) {
          if (tableau[column].size + 1 == row) {
            printf("\e[" mFGCOLOR mGREEN "m" uBOTTOMLEFT "\e[m");
          } else if (tableau[column].size - selection.size == row + 1) {
            printf("\e[" mFGCOLOR mGREEN "m" uTOPLEFT "\e[m");
          } else {
            printf(" ");
          }
        }

        if (row < tableau[column].size) {
          if (row + 1 < tableau[column].size) {
            printf("\e[" mUNDERLINE "m");
          }
          print_card_top(
              tableau[column].numFlipped > row ? 0 : tableau[column].cards[row],
              selection.ptr.card_pile == &tableau[column] &&
                  tableau[column].size - selection.size <= row);
          again = true;
        } else if (row && row == tableau[column].size) {
          bool selected = selection.ptr.card_pile == &tableau[column] &&
                          tableau[column].size - selection.size <= row + 1;
          print_card_bottom(tableau[column].numFlipped >= row
                                ? 0
                                : tableau[column].cards[row - 1],
                            selected);
          again = again || selected;
        } else {
          printf("  ");
        }
      } else {
        printf(" ");
      }
    }
  }
  printf("\n");
}
