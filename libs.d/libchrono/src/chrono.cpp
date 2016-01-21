#include "lchrono/chrono.h"
#include <time.h>
#include <sys/time.h>

namespace {
    struct timeval m_timeref = {.tv_sec = -1, .tv_usec = 0};
}

lchrono::timepoint lchrono::clock() {
    struct timeval tv;
    gettimeofday(&tv, 0);

    return std::chrono::nanoseconds{(tv.tv_sec - m_timeref.tv_sec) * 1000000000ULL + (tv.tv_usec - m_timeref.tv_usec) * 1000};
}