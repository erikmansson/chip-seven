#ifndef HELPERS_H
#define HELPERS_H

#include <time.h>

#define NS_IN_1_S 1000000000L

long delta_time_ns(struct timespec *t0, struct timespec *t1);

#endif
