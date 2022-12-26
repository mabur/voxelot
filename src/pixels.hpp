#pragma once

#include <cstdint>

#include "color.hpp"
#include "array2.hpp"

using PackedColor = uint32_t;
using Pixels = Array2<PackedColor>;

PackedColor packColor(const Color& color);
