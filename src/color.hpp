#pragma once

#include <cstdint>

using PackedColor = uint32_t;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color operator+(uint8_t x) const;
    Color operator-(uint8_t x) const;
    PackedColor pack() const;
};

const auto BLACK = Color{0, 0, 0};
const auto WHITE = Color{255, 255, 255};
const auto GRAY64 = Color{64, 64, 64};
const auto GRAY128 = Color{128, 128, 128};
const auto GRAY192 = Color{192, 192, 192};
const auto RED = Color{55, 0, 0};
const auto GREEN = Color{0, 255, 0};
