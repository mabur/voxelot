#pragma once

#include <valarray>

template<typename T>
class Image {
public:
    Image(size_t width, size_t height)
        : width_{ width }
        , height_{ height }
        , array_(width * height)
    {}

    size_t width() const { return width_; }
    size_t height() const { return height_; }
    size_t size() const { return width_ * height_; }

    T& operator()(size_t x, size_t y) {return array_[y * width_ + x];}

    T* data() { return std::begin(array_); }
    const T* data() const { return std::begin(array_); }
    T* begin() { return data(); }
    const T* begin() const { return data(); }
    T* end() { return data() + size(); }
    const T* end() const { return data() + size(); }

private:
    size_t width_;
    size_t height_;
    std::valarray<T> array_;
};
