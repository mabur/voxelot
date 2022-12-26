#include "button.hpp"

#include <algorithm>

Uint8 addUint8(Uint8 a, Uint8 b) {
    return static_cast<Uint8>(
        std::min(static_cast<int>(a) + static_cast<int>(b), 255)
    );
}

Uint8 subUint8(Uint8 a, Uint8 b) {
    return static_cast<Uint8>(
        std::max(static_cast<int>(a) - static_cast<int>(b), 0)
    );
}

void drawButton(const Button& button, Pixels& pixels) {
    if (!button.visible) return;

    const auto xmin = button.rectangle.x;
    const auto ymin = button.rectangle.y;
    const auto xmax = button.rectangle.x + button.rectangle.w - 1;
    const auto ymax = button.rectangle.y + button.rectangle.h - 1;

    const auto COLOR_DELTA = 40;

    const auto r = button.r;
    const auto g = button.g;
    const auto b = button.b;

    const auto r_bright = addUint8(r, COLOR_DELTA);
    const auto g_bright = addUint8(g, COLOR_DELTA);
    const auto b_bright = addUint8(b, COLOR_DELTA);

    const auto r_brightest = addUint8(r, 2 * COLOR_DELTA);
    const auto g_brightest = addUint8(g, 2 * COLOR_DELTA);
    const auto b_brightest = addUint8(b, 2 * COLOR_DELTA);

    const auto r_dark = subUint8(r, COLOR_DELTA);
    const auto g_dark = subUint8(g, COLOR_DELTA);
    const auto b_dark = subUint8(b, COLOR_DELTA);

    const auto r_darkest = subUint8(r, 2 * COLOR_DELTA);
    const auto g_darkest = subUint8(g, 2 * COLOR_DELTA);
    const auto b_darkest = subUint8(b, 2 * COLOR_DELTA);

    const auto color = Color{r, g, b}.pack();
    const auto color_dark = Color{r_dark, g_dark, b_dark}.pack();
    const auto color_darkest = Color{r_darkest, g_darkest, b_darkest}.pack();
    const auto color_bright = Color{r_bright, g_bright, b_bright}.pack();
    const auto color_brightest = Color{r_brightest, g_brightest, b_brightest}.pack();

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
