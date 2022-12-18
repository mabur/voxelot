#define SDL_MAIN_HANDLED

#include "algorithm.hpp"
#include "camera.hpp"
#include "drawing.hpp"
#include "sdl_wrappers.hpp"
#include "world.hpp"

template<typename T>
T clamp(T x, T low, T high) {
    return std::min(std::max(x, low), high);
}

CameraExtrinsics translate(CameraExtrinsics extrinsics, const Input& input) {
    const auto V = 0.05;
    const auto world_from_camera = worldFromCamera(extrinsics);
    const auto& keyboard = input.keyboard;
    auto velocity_in_world = Vector4d{ 0,0,0,0 };
    if (keyboard[SDL_SCANCODE_D]) {
        velocity_in_world += world_from_camera * Vector4d{ +V,0,0,0 };
    }
    if (keyboard[SDL_SCANCODE_A]) {
        velocity_in_world += world_from_camera * Vector4d{ -V,0,0,0 };
    }
    if (keyboard[SDL_SCANCODE_W]) {
        velocity_in_world += world_from_camera * Vector4d{ 0,0,+V,0 };
    }
    if (keyboard[SDL_SCANCODE_S]) {
        velocity_in_world += world_from_camera * Vector4d{ 0,0,-V,0 };
    }
    if (keyboard[SDL_SCANCODE_Q]) {
        velocity_in_world += Vector4d{ 0,+V,0,0 };
    }
    if (keyboard[SDL_SCANCODE_E]) {
        velocity_in_world += Vector4d{ 0,-V,0,0 };
    }
    extrinsics.x += velocity_in_world.x();
    extrinsics.y += velocity_in_world.y();
    extrinsics.z += velocity_in_world.z();
    return extrinsics;
}

CameraExtrinsics rotate(CameraExtrinsics extrinsics, const Input& input) {
    const auto angular_velocity = 0.1 * 3.14 / 180.0;
    extrinsics.yaw += input.mouse_dx * angular_velocity;
    extrinsics.pitch -= input.mouse_dy * angular_velocity;
    extrinsics.pitch = clamp(extrinsics.pitch, -3.14 * 0.5, +3.14 * 0.5);
    return extrinsics;
}

CameraExtrinsics updateCamera(CameraExtrinsics extrinsics, const Input& input) {
    extrinsics = translate(extrinsics, input);
    extrinsics = rotate(extrinsics, input);
    return extrinsics;
}

int main(int, char**)
{
    const auto WINDOW_TITLE = "voxelot";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;

    auto pixels = Pixels(WIDTH, HEIGHT, BLACK);
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);

    World world = makeWorld(sdl.width, sdl.height);

    while (sdl.noQuitMessage()) {
        const auto input = sdl.getInput();
        if (input.escape_button == ButtonState::CLICKED) {
            return 0;
        }
        world.timer.update();
        world.extrinsics = updateCamera(world.extrinsics, input);
        draw(pixels, world);
        sdl.draw(pixels.data());

    }
    return 0;
}
