#include "environment.hpp"

Environment makeEnvironment(size_t screen_width, size_t screen_height) {
    auto environment = Environment{};
    environment.map = Map{};
    environment.intrinsics = makeCameraIntrinsics(screen_width, screen_height);
    environment.extrinsics = makeCameraExtrinsics();
    return environment;
}
