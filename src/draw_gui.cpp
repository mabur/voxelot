#include "draw_gui.hpp"

#include <string>

#include "button.hpp"
#include "color.hpp"
#include "text.hpp"

namespace {

void drawCharacter(
    Pixels& pixels, char character, size_t x_start, size_t y_start, PackedColor color
) {
    const auto& digit_bitmap = text_bitmaps::get(character);
    for (size_t y = 0; y < 8; ++y) {
        for (size_t x = 0; x < 8; ++x) {
            if (digit_bitmap[y * 8 + x]) {
                pixels(x_start + x, y_start + y) = color;
            }
        }
    }
}

}; // namespace

void drawString(
    Pixels& pixels, const std::string& s, size_t x, size_t y, PackedColor color
) {
    for (size_t i = 0; i < s.size(); ++i) {
        drawCharacter(pixels, s[i], x + 8 * i, y, color);
    }
}

void drawButton(Pixels& pixels, const Button& button) {
    if (!button.visible) return;

    const auto xmin = button.rectangle.x;
    const auto ymin = button.rectangle.y;
    const auto xmax = button.rectangle.x + button.rectangle.w - 1;
    const auto ymax = button.rectangle.y + button.rectangle.h - 1;

    const auto COLOR_DELTA = 40;

    const auto color_text = packColor(WHITE);
    const auto color = packColor(button.color);
    const auto color_bright = packColor(button.color + COLOR_DELTA);
    const auto color_brightest = packColor(button.color + 2 * COLOR_DELTA);
    const auto color_dark = packColor(button.color - COLOR_DELTA);
    const auto color_darkest = packColor(button.color - 2 * COLOR_DELTA);

    if (button.selected) {
        for (auto y = ymin; y <= ymax; ++y) {
            for (auto x = xmin; x <= xmax; ++x) {
                pixels(x, y) = color;
            }
        }
        drawString(pixels, button.text, xmin + 4, ymin + 5, color_text);
    }
    else {
        for (auto y = ymin; y <= ymax; ++y) {
            for (auto x = xmin; x <= xmax; ++x) {
                if (x == xmin || y == ymin) {
                    pixels(x, y) = color_bright;
                }
                else if (x == xmax || y == ymax) {
                    pixels(x, y) = color_dark;
                }
                else {
                    pixels(x, y) = color;
                }
            }
            pixels(xmin, ymin) = color_brightest;
            pixels(xmax, ymax) = color_darkest;
            pixels(xmin, ymax) = color;
            pixels(xmax, ymin) = color;
        }
        drawString(pixels, button.text, xmin + 4, ymin + 4, color_text);
    }

}
