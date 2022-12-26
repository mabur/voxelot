#pragma once

#include <string>
#include <valarray>
#include <vector>

#include "array2.hpp"
#include "color.hpp"
#include "pixels.hpp"

struct World;

void draw(Pixels& pixels, const World& world);
