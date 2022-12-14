#pragma once

#include "vector_space.hpp"
#include "voxels.hpp"

const auto MAP_SIZE = 8;

// x right
// y up
// z backward
//
// One corner of the Voxel volume is in the origin.
// The (x,y,z) indices of a voxel correspond to the coordinates of
// the corner that is closest to the origin,
// in the grid coordinate system.
// The world coordinate system has the same origin but different scale.
struct Map {
    Voxels voxels;
    double voxel_length = 1.0;
    Map();

    size_t widthGrid() const { return voxels.width(); }
    size_t heightGrid() const { return voxels.height(); }
    size_t depthGrid() const { return voxels.depth(); }

    Matrix4d worldFromGrid() const;
    Matrix4d gridFromWorld() const;
    Vector4d centerInGrid() const;
    Vector4d centerInWorld() const;
    Vector4s closestVoxelInGrid(const Vector4d& position_in_world) const;
    Vector4d voxelCenterInWorld(const Vector4s& voxel_in_grid) const;    
};
