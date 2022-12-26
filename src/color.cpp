#include "color.hpp"

PackedColor Color::pack() const {
    return 
        (uint32_t{255} << 24) |
        (uint32_t{r} << 16) |
        (uint32_t{g} << 8) |
        (uint32_t{b} << 0);
}
