#pragma once

#include <cstdint>

using PackedColor = uint32_t;

PackedColor packColorRgb(uint32_t r, uint32_t g, uint32_t b);

const auto BLACK = packColorRgb(0, 0, 0);
const auto WHITE = packColorRgb(255, 255, 255);
const auto GRAY64 = packColorRgb(64, 64, 64);
const auto GRAY128 = packColorRgb(128, 128, 128);
const auto GRAY192 = packColorRgb(192, 192, 192);
const auto RED = packColorRgb(255, 0, 0);
const auto GREEN = packColorRgb(0, 255, 0);
