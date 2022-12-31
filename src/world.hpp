#pragma once

#include <map>

#include "camera.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "timer.hpp"
#include "vector_space.hpp"

struct World {
    CameraIntrinsics intrinsics;
    CameraExtrinsics extrinsics;
    std::map<BuildingBlock,Map> building_blocks;
    BuildingBlock active_building_block = BuildingBlock::B1;
    FrequencyTimer timer;
};

World makeWorld(size_t screen_width, size_t screen_height);

Vector4s selectedVoxel(const World& world);
