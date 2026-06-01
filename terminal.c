#include "terminal.h"
#include "card.h"
#include "main.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef MSWIN
#include <windows.h>

static DWORD outer_settings;
static UINT encoding;

void init_terminal(void)
{
  GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &outer_settings);
  encoding = GetConsoleOutputCP();
  DWORD settings = outer_settings & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), settings);
  SetConsoleOutputCP(65001);
  atexit(exit_terminal);
}
void exit_terminal(void) { SetConsoleMode(STDIN_FILENO, outer_settings); }
#else
#include <termios.h>
#include <unistd.h>

static struct termios outer_settings;

void init_terminal(void)
{
  tcgetattr(STDIN_FILENO, &outer_settings);
  struct termios settings = outer_settings;
  settings.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &settings);
  atexit(exit_terminal);
}
void exit_terminal(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &outer_settings); }
#endif

void clear_screen(void) { fputs("\e[2J\e[3J\e[;H", stdout); }

void setup_card_colors(struct Card card, bool selected)
{
  putchar(*mFGCOLOR);
  switch (COLOR(card.suit))
  {
  case BLACK:
    putchar(*mBLACK);
    break;
  case RED:
    putchar(*mRED);
    break;
  }
  fputs(";" mBGCOLOR, stdout);
  putchar(selected ? *mGREEN : *mWHITE);
  putchar('m');
}

void print_card_top(struct Card card, bool selected)
{
  fputs("\e[", stdout);
  if (CARDEXISTS(card))
  {
    setup_card_colors(card, selected);
    static const char rank_chars[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', '1', 'J', 'Q', 'K'};
    putchar(rank_chars[card.rank - 1]);
    static const char suit_chars[][4] = {uSPADES, uCLUBS, uHEARTS, uDIAMONDS};
    fputs(suit_chars[card.suit], stdout);
  }
  else
  {
    fputs(mFGCOLOR mWHITE ";" mBGCOLOR mRED "m" uSPADES uHEARTS, stdout);
  }
  fputs("\e[m", stdout);
}

void print_card_bottom(struct Card card, bool selected)
{
  fputs("\e[", stdout);
  if (CARDEXISTS(card))
  {
    setup_card_colors(card, selected);
    fputs(card.rank == 10 ? "0 " : "  ", stdout);
  }
  else
  {
    fputs(mFGCOLOR mWHITE ";" mBGCOLOR mRED "m" uDIAMONDS uCLUBS, stdout);
  }
  fputs("\e[m", stdout);
}

void refresh_screen(void)
{
  clear_screen();

  // row 1
  fputs(" Q ", stdout);
  fputs(selection.ptr.card_pile == &drawPile ? "\e[32m" uTOPLEFT "\e[mW \e[32m" uTOPRIGHT "\e[m" : " W  ", stdout);
  fputs("E ", stdout);
  for (size_t index = 0; index <= len(foundation); index++)
  {
    static const char keys[4] = {'U', 'I', 'O', 'P'};
    if (index && selection.ptr.card == &foundation[index - 1])
    {
      fputs("\e[" mFGCOLOR mGREEN "m" uTOPRIGHT "\e[m", stdout);
    }
    else if (index < len(foundation) && selection.ptr.card == &foundation[index])
    {
      fputs("\e[" mFGCOLOR mGREEN "m" uTOPLEFT "\e[m", stdout);
    }
    else
    {
      putchar(' ');
    }
    if (index < len(foundation))
    {
      putchar(keys[index]);
      putchar(' ');
    }
  }

  // row 2
  fputs("\n ", stdout);
  if (drawPile.numFlipped)
  {
    print_card_top(NILCARD, false);
  }
  else
  {
    fputs("  ", stdout);
  }
  putchar(' ');
  if (drawPile.numFlipped < drawPile.size)
  {
    print_card_top(top((union Selectable){.card_pile = &drawPile}), selection.ptr.card_pile == &drawPile);
  }
  else
  {
    fputs("  ", stdout);
  }
  fputs(" \e[" mFGCOLOR mRED "m" uBSLASH uFSLASH "\e[m ", stdout);
  for (struct Card const *f = &foundation[0]; f < end(foundation); f++)
  {
    if (CARDEXISTS(*f))
    {
      print_card_top(*f, selection.ptr.card == f);
    }
    else
    {
      fputs("  ", stdout);
    }
    putchar(' ');
  }

  // row 3
  fputs("\n ", stdout);
  if (drawPile.numFlipped)
  {
    print_card_bottom(NILCARD, false);
  }
  else
  {
    fputs("  ", stdout);
  }
  putchar(' ');
  if (drawPile.numFlipped < drawPile.size)
  {
    print_card_bottom(top((union Selectable){.card_pile = &drawPile}), selection.ptr.card_pile == &drawPile);
  }
  else
  {
    fputs("  ", stdout);
  }
  fputs(" \e[" mFGCOLOR mRED "m" uFSLASH uBSLASH "\e[m ", stdout);
  for (struct Card const *f = &foundation[0]; f < end(foundation); f++)
  {
    if (CARDEXISTS(*f))
    {
      print_card_bottom(*f, selection.ptr.card == f);
    }
    else
    {
      fputs("  ", stdout);
    }
    putchar(' ');
  }

  // row 4
  putchar('\n');
  if (selection.ptr.card)
  {
    static const union Selectable upper_selections[9] = {{.card = NULL}, {.card = NULL}, {.card_pile = &drawPile}, {.card = NULL}, {.card = &foundation[0]}, {.card = &foundation[1]}, {.card = &foundation[2]}, {.card = &foundation[3]}, {.card = NULL}};
    for (size_t i = 0; i <= len(tableau); i++)
    {
      if (selection.ptr.card == upper_selections[i].card)
      {
        fputs("\e[" mFGCOLOR mGREEN "m" uBOTTOMRIGHT "\e[m", stdout);
      }
      else if (selection.ptr.card == upper_selections[i + 1].card)
      {
        fputs("\e[" mFGCOLOR mGREEN "m" uBOTTOMLEFT "\e[m", stdout);
      }
      else if (i && selection.ptr.card_pile == &tableau[i - 1] && selection.size == tableau[i - 1].size)
      {
        fputs("\e[" mFGCOLOR mGREEN "m" uTOPRIGHT "\e[m", stdout);
      }
      else if (i < 7 && selection.ptr.card_pile == &tableau[i] && selection.size == tableau[i].size)
      {
        fputs("\e[" mFGCOLOR mGREEN "m" uTOPLEFT "\e[m", stdout);
      }
      else
      {
        putchar(' ');
      }
      if (i < 7)
      {
        static const char keys[7] = {'A', 'S', 'D', 'J', 'K', 'L', ';'};
        putchar(keys[i]);
        putchar(' ');
      }
    }
  }
  else
  {
    fputs(" A  S  D  J  K  L  ;  ", stdout);
  }

  bool again = true;
  for (size_t row = 0; again; row++)
  {
    again = false;
    putchar('\n');
    for (size_t column = 0; column <= len(tableau); column++)
    {
      if (column && selection.ptr.card_pile == &tableau[column - 1])
      {
        if (tableau[column - 1].size + 1 == row)
        {
          fputs("\e[" mFGCOLOR mGREEN "m" uBOTTOMRIGHT "\e[m", stdout);
        }
        else if (tableau[column - 1].size - selection.size == row + 1)
        {
          fputs("\e[" mFGCOLOR mGREEN "m" uTOPRIGHT "\e[m", stdout);
        }
        else
        {
          putchar(' ');
        }
      }
      else if (column < len(tableau) && selection.ptr.card_pile == &tableau[column])
      {
        if (tableau[column].size + 1 == row)
        {
          fputs("\e[" mFGCOLOR mGREEN "m" uBOTTOMLEFT "\e[m", stdout);
        }
        else if (tableau[column].size - selection.size == row + 1)
        {
          fputs("\e[" mFGCOLOR mGREEN "m" uTOPLEFT "\e[m", stdout);
        }
        else
        {
          putchar(' ');
        }
      }
      else
      {
        putchar(' ');
      }
      if (column < len(tableau))
      {
        if (row < tableau[column].size)
        {
          if (row + 1 < tableau[column].size)
          {
            fputs("\e[" mUNDERLINE "m", stdout);
          }
          print_card_top(tableau[column].numFlipped > row ? NILCARD : tableau[column].cards[row], selection.ptr.card_pile == &tableau[column] && tableau[column].size - selection.size <= row);
          again = true;
        }
        else if (row && row == tableau[column].size)
        {
          bool selected = selection.ptr.card_pile == &tableau[column] && tableau[column].size - selection.size <= row + 1;
          print_card_bottom(tableau[column].numFlipped >= row ? NILCARD : tableau[column].cards[row - 1], selected);
          again = again || selected;
        }
        else
        {
          fputs("  ", stdout);
        }
      }
      else
      {
        putchar(' ');
      }
    }
  }
  putchar('\n');
}
