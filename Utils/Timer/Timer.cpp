//
// Created by nelep on 27.11.2025.
//

#include "Timer.h"

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

long long Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
}
