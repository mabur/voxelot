#include "environment.hpp"

Environment makeEnvironment(size_t screen_width, size_t screen_height) {
    const auto map = Map();
    auto environment = Environment(map);
    environment.intrinsics = makeCameraIntrinsics(screen_width, screen_height);
    environment.extrinsics = CameraExtrinsics{
        environment.position_start.x(),
        environment.position_start.y(),
        environment.position_start.z(),
        0.0,
        0.0
    };
    return environment;
}

Vector4d cameraViewPosition(const CameraExtrinsics& coordinates) {
    return worldFromCamera(coordinates) * Vector4d { 0, 0, 2 * L, 1 };
}
