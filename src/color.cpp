#include "color.hpp"

#include <algorithm>

uint8_t addUint8(uint8_t a, uint8_t b) {
    return static_cast<uint8_t>(
        std::min(static_cast<int>(a) + static_cast<int>(b), 255)
    );
}

uint8_t subUint8(uint8_t a, uint8_t b) {
    return static_cast<uint8_t>(
        std::max(static_cast<int>(a) - static_cast<int>(b), 0)
    );
}

Color Color::operator+(uint8_t x) const {
    return Color{addUint8(r, x), addUint8(g, x), addUint8(b, x)};
}

Color Color::operator-(uint8_t x) const {
    return Color{subUint8(r, x), subUint8(g, x), subUint8(b, x)};
}
