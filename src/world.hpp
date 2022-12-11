#pragma once

#include "camera.hpp"
#include "map.hpp"
#include "timer.hpp"

struct World {
    CameraIntrinsics intrinsics;
    CameraExtrinsics extrinsics;
    Map map;
    FrequencyTimer timer;
};

World makeWorld(size_t screen_width, size_t screen_height);
