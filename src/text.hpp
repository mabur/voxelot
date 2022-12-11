#pragma once

#include <array>

namespace text_bitmaps {
    using Bitmap = std::array<int, 8 * 8>;
    Bitmap get(char character);
}
