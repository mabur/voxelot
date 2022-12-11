#include "world.hpp"

World makeWorld(size_t screen_width, size_t screen_height) {
    auto world = World{};
    world.map = Map{};
    world.intrinsics = makeCameraIntrinsics(screen_width, screen_height);
    world.extrinsics = makeCameraExtrinsics();
    return world;
}
