#include "pixels.hpp"

PackedColor packColor(const Color& color) {
    return
        (uint32_t{255} << 24) |
        (uint32_t{color.r} << 16) |
        (uint32_t{color.g} << 8) |
        (uint32_t{color.b} << 0);
}
