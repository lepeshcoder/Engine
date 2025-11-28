//
// Created by nelep on 27.11.2025.
//

#ifndef ENGINE_TIMER_H
#define ENGINE_TIMER_H
#include <iostream>
#include <chrono>


class Timer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

public:
    void start();

    // Возвращает время в микросекундах
    long long stop();
};

#endif //ENGINE_TIMER_H