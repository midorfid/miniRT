#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define RANDOM_MAX UINT64_MAX
#define RANDOM_DEFAULT_SEED 5489

void random_seed(uint64_t seed);

uint64_t random(void);

#endif