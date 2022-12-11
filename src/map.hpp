#pragma once

#include <set>
#include <string>
#include <vector>

#include "array3.hpp"
#include "vector_space.hpp"

const auto MAP_SIZE = 3;

inline int roundDoubleToInt(double x) {
    return static_cast<int>(std::round(x));
}

// x right
// y up
// z backward
struct Map {
    Array3<int> voxels;
    double voxel_length = 1.0;
    Map();

    size_t widthGrid() const { return voxels.width(); }
    size_t heightGrid() const { return voxels.height(); }
    size_t depthGrid() const { return voxels.depth(); }

    Matrix4d worldFromGrid() const;
    Matrix4d gridFromWorld() const;
    Vector4d centerInGrid() const;
    Vector4d centerInWorld() const;
    Vector4d closestVoxelCenterInWorld(const Vector4d& position_in_world) const;
};
