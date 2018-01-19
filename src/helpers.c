#include <helpers.h>

long delta_time_ns(struct timespec *t0, struct timespec *t1)
{
    return
        (long)(t1->tv_sec - t0->tv_sec) * NS_IN_1_S +
        (long)(t1->tv_nsec - t0->tv_nsec);
}
