#include "timer.hpp"

void FrequencyTimer::update() {
    const auto new_time_point = Clock::now();
    latest_duration = new_time_point - latest_time_point;
    latest_time_point = new_time_point;
}

size_t FrequencyTimer::frequency() const {
    return Clock::period::den / Clock::period::num / latest_duration.count();
}
