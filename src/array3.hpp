#pragma once

#include <valarray>

template<typename T>
class Array3 {
public:
    Array3()
        : width_{0}
        , height_{0}
        , depth_{0}
        , array_(0)
    {}
    Array3(size_t width, size_t height, size_t depth, T value)
        : width_{ width }
        , height_{ height }
        , depth_{ height }
        , array_(value, width * height * depth)
    {}

    size_t width() const { return width_; }
    size_t height() const { return height_; }
    size_t depth() const { return depth_; }
    size_t size() const { return width_ * height_ * depth_; }

    T& operator()(size_t x, size_t y, size_t z) {
        return array_[z * width_ * height_ + y * width_ + x];
    }

    T* data() { return std::begin(array_); }
    const T* data() const { return std::begin(array_); }
    T* begin() { return data(); }
    const T* begin() const { return data(); }
    T* end() { return data() + size(); }
    const T* end() const { return data() + size(); }

private:
    size_t width_;
    size_t height_;
    size_t depth_;
    std::valarray<T> array_;
};
