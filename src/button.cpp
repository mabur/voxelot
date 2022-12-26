#include "button.hpp"

void drawButton(const Button& button, Pixels& pixels) {
    if (!button.visible) return;

    const auto xmin = button.rectangle.x;
    const auto ymin = button.rectangle.y;
    const auto xmax = button.rectangle.x + button.rectangle.w - 1;
    const auto ymax = button.rectangle.y + button.rectangle.h - 1;

    const auto COLOR_DELTA = 40;

    const auto color = (button.color).pack();
    const auto color_bright = (button.color + COLOR_DELTA).pack();
    const auto color_brightest = (button.color + 2 * COLOR_DELTA).pack();
    const auto color_dark = (button.color - COLOR_DELTA).pack();
    const auto color_darkest = (button.color - 2 * COLOR_DELTA).pack();

    if (button.selected) {
        for (auto y = ymin; y <= ymax; ++y) {
            for (auto x = xmin; x <= xmax; ++x) {
                pixels(x, y) = color;
            }
        }
        drawString(pixels, button.text, xmin + 4, ymin + 5, WHITE.pack());
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
        drawString(pixels, button.text, xmin + 4, ymin + 4, WHITE.pack());
    }
    
}
