#pragma once

#include "camera.hpp"
#include "map.hpp"
#include "timer.hpp"

struct Environment {
    CameraIntrinsics intrinsics;
    CameraExtrinsics extrinsics;
    Map map;
    FrequencyTimer timer;
};

Environment makeEnvironment(size_t screen_width, size_t screen_height);
