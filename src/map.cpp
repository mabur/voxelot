#include "map.hpp"

#include "algorithm.hpp"

template<typename T>
T clamp(T x, T low, T high) {
    return std::min(std::max(x, low), high);
}

size_t roundDoubleToSize(double x) {
    return static_cast<size_t>(std::max(std::round(x), 0.0));
}

Map::Map() {
    voxels = Array3<int>(MAP_SIZE, MAP_SIZE, MAP_SIZE, 0);
}

Vector4d Map::centerInWorld() const {
    return Vector4d{
        0.5 * voxel_length * widthGrid(),
        0.5 * voxel_length * heightGrid(),
        0.5 * voxel_length * depthGrid(),
        1.0
    };
}

Vector4d Map::closestVoxelCenterInWorld(const Vector4d& position_in_world) const {
    const auto x = position_in_world.x() / voxel_length;
    const auto y = position_in_world.y() / voxel_length;
    const auto z = position_in_world.z() / voxel_length;

    const auto xi = clamp<size_t>(roundDoubleToSize(x), 0, widthGrid() - 1);
    const auto yi = clamp<size_t>(roundDoubleToSize(y), 0, heightGrid() - 1);
    const auto zi = clamp<size_t>(roundDoubleToSize(z), 0, depthGrid() - 1);

    return Vector4d{
        xi * voxel_length,
        yi * voxel_length,
        zi * voxel_length,
        1.0
    };
}
