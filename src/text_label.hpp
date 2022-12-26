#pragma once

#include <string>

#include "color.hpp"

struct TextLabel {
    std::string text;
    Color color;
    int x;
    int y;
};
