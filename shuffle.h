#ifndef _SHUFFLEH_
#define _SHUFFLEH_

#include "card.h"

/**
 * @brief generate a random number with a maximum value
 * 
 * Uses rejection sampling to avoid bias.
 */
unsigned int brand(unsigned int max);

void new_shuffle();

#endif
