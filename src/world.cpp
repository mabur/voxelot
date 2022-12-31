#include "world.hpp"

World makeWorld(size_t screen_width, size_t screen_height) {
    auto world = World{};
    for (auto i = 0; i < static_cast<int>(BuildingBlock::COUNT); ++i) {
        world.building_blocks[static_cast<BuildingBlock>(i)] = Map{};
    }
    const auto camera_position = (
        world.building_blocks[BuildingBlock::B1].centerInWorld() +
        Vector4d{0, 0, 10, 0}
    ).eval();
    world.intrinsics = makeCameraIntrinsics(screen_width, screen_height);
    world.extrinsics = makeCameraExtrinsics(camera_position);
    return world;
}

Vector4s selectedVoxel(const World& world) {
    const auto& map = world.building_blocks.at(world.active_building_block);
    const auto camera_view_position = (
        worldFromCamera(world.extrinsics) *
        Vector4d{0, 0, 2 * map.voxel_length, 1}
    ).eval();
    return map.closestVoxelInGrid(camera_view_position);
}
