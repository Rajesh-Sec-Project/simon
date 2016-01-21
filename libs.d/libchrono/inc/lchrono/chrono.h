// Created by remi on 21/01/2016

#ifndef LCHRONO_HEADER
#define LCHRONO_HEADER

#include <chrono>

using namespace std::literals;

namespace lchrono {
    using duration = std::chrono::nanoseconds;
    using timepoint = std::chrono::nanoseconds;

    //! Return the elasped time since the first call of this function.
    timepoint clock();
}

#endif
