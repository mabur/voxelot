#include "world.hpp"

World makeWorld(size_t screen_width, size_t screen_height) {
    auto world = World{};
    world.map = Map{};
    world.intrinsics = makeCameraIntrinsics(screen_width, screen_height);
    world.extrinsics = makeCameraExtrinsics(world.map.centerInWorld());
    return world;
}

Vector4s selectedVoxel(const World& world) {
    const auto& map = world.map;
    const auto camera_view_position = (
        worldFromCamera(world.extrinsics) *
        Vector4d{0, 0, 2 * map.voxel_length, 1}
    ).eval();
    return map.closestVoxelInGrid(camera_view_position);
}
