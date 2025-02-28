#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../random.h"

#ifdef M_PI
#define PI M_PI
#else
#define PI (double)3.1415926535897932385
#endif

#define DEG_TO_RAD(deg) (((deg)*PI) / 180.0)

static inline double random_double(double min, double max)
{
    return min + (max - min) * drand48() / (RANDOM_MAX + 0.0);
}

static inline double random_double_nolimits(void)
{
    return drand48() / (RANDOM_MAX + 0.0);
}

static inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}


#endif