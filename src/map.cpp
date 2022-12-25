#include "map.hpp"

#include "algorithm.hpp"

template<typename T>
T clamp(T x, T low, T high) {
    return std::min(std::max(x, low), high);
}

size_t sizeFromDouble(double x) {
    return x < 0 ? 0 : static_cast<size_t>(x);
}

size_t roundDoubleToSize(double x) {
    return static_cast<size_t>(std::max(std::round(x), 0.0));
}

Map::Map() {
    voxels = Array3<int>(MAP_SIZE, MAP_SIZE, MAP_SIZE, 0);
    const auto M = MAP_SIZE - 1;
    voxels(0, 0, 0) = 1;
    voxels(M, 0, 0) = 1;
    voxels(0, M, 0) = 1;
    voxels(0, 0, M) = 1;
    voxels(0, M, M) = 1;
    voxels(M, 0, M) = 1;
    voxels(M, M, 0) = 1;
    voxels(M, M, M) = 1;
}

Matrix4d Map::worldFromGrid() const {
    auto world_from_grid = Matrix4d{voxel_length * Matrix4d::Identity()};
    world_from_grid(3, 3) = 1.0;
    return world_from_grid;
}

Matrix4d Map::gridFromWorld() const {
    return worldFromGrid().inverse();
}

Vector4d Map::centerInGrid() const {
    return Vector4d{
        0.5 * widthGrid(),
        0.5 * heightGrid(),
        0.5 * depthGrid(),
        1.0
    };
}

Vector4d Map::centerInWorld() const {
    return worldFromGrid() * centerInGrid();
}

Vector4s Map::closestVoxelInGrid(const Vector4d& position_in_world) const {
    const auto p = (gridFromWorld() * position_in_world).eval();
    return Vector4s {
        clamp<size_t>(sizeFromDouble(std::round(p.x())), 0, widthGrid() - 1),
        clamp<size_t>(sizeFromDouble(std::round(p.y())), 0, heightGrid() - 1),
        clamp<size_t>(sizeFromDouble(std::round(p.z())), 0, depthGrid() - 1),
        1
    };
}

Vector4d Map::voxelCenterInWorld(const Vector4s& voxel_in_grid) const {
    return worldFromGrid() * (voxel_in_grid.cast<double>()+ Vector4d{0.5, 0.5, 0.5, 0.0});
}
