#pragma once

#include <vector>

#include "color.hpp"

struct World;

struct Pixels {
    std::vector<Color> colors;
    size_t width;
    size_t height;

    Pixels(int width, int height);
    size_t size() const;
    void clear();
};

void drawBuildMode(Pixels& pixels, const World& world);
