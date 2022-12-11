#pragma once

#include <chrono>

struct FrequencyTimer {
    void update();
    size_t frequency() const;
private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point latest_time_point;
    Clock::duration latest_duration;
};
