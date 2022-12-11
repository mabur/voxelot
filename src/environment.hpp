#pragma once

#include <string>
#include <vector>

#include "camera.hpp"
#include "map.hpp"
#include "timer.hpp"
#include "vector_space.hpp"

struct Environment {
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    explicit Environment(const Map& map) : map{ map } {}
    CameraIntrinsics intrinsics;
    CameraExtrinsics extrinsics;
    Map map;
    FrequencyTimer timer;
};

Environment makeEnvironment(size_t screen_width, size_t screen_height);
Vector4d cameraViewPosition(const CameraExtrinsics& coordinates);

