#pragma once

#include <valarray>
#include <vector>

#include "color.hpp"
#include "Image.hpp"

struct World;

using Pixels = Image<Color>;

void drawBuildMode(Pixels& pixels, const World& world);
