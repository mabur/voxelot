#pragma once

#include <vector>

#include "color.hpp"

struct World;

struct Pixels {
    Pixels(size_t width, size_t height)
        : width_{width}
        , height_{height}
        , colors_(width * height)
    {}
    
    size_t width() const {return width_;}
    size_t height() const {return height_;}
    size_t size() const {return width_ * height_;}

    Color& operator()(size_t x, size_t y) {
        return colors_.at(y * width_ + x);
    }

    Color* data() {return colors_.data();}
    const Color* data() const {return colors_.data();}
    Color* begin() {return data();}
    const Color* begin() const {return data();}
    Color* end() {return data() + size();}
    const Color* end() const {return data() + size();}

private:
    size_t width_;
    size_t height_;
    std::vector<Color> colors_;
};

void drawBuildMode(Pixels& pixels, const World& world);
