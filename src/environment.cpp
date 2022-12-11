#include "environment.hpp"

Environment makeEnvironment(size_t screen_width, size_t screen_height) {
    const auto map = Map();
    auto environment = Environment(map);
    environment.intrinsics = makeCameraIntrinsics(screen_width, screen_height);
    environment.extrinsics = makeCameraExtrinsics();
    return environment;
}
