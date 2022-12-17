#pragma once

#include <valarray>
#include <vector>

#include "color.hpp"
#include "array2.hpp"

struct World;

using Pixels = Array2<Color>;

void draw(Pixels& pixels, const World& world);
