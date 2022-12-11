#include "map.hpp"

#include "algorithm.hpp"

template<typename T>
T clamp(T x, T low, T high) {
    return std::min(std::max(x, low), high);
}

Map::Map()
{
    voxels = Array3<int>(MAP_SIZE, MAP_SIZE, MAP_SIZE, 0);
}

int Map::blockIndexX(Vector4d position_world) const {
    const auto z = blockIndexZ(position_world);
    const auto x = roundDoubleToInt(position_world.x() / voxel_length);
    return clamp(x, 0, MAP_SIZE - 1);
}
int Map::blockIndexZ(Vector4d position_world) const {
    const auto z = roundDoubleToInt(position_world.z() / voxel_length);
    return clamp(z, 0, MAP_SIZE - 1);
}

Vector4d Map::blockPositionCenter(int z, int y, int x) const {
    return Vector4d{ x * voxel_length, y * voxel_length, z * voxel_length, 1.0 };
}
