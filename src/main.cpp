#define SDL_MAIN_HANDLED

#include "camera.hpp"
#include "drawing.hpp"
#include "sdl_wrappers.hpp"
#include "environment.hpp"

const auto velocity = 0.05;
const auto angular_velocity = 0.1 * 3.14 / 180.0;

template<typename T>
T clamp(T x, T low, T high)
{
    return std::min(std::max(x, low), high);
}

Vector4d velocityInCamera(const Input& input) {
    const auto& keyboard = input.keyboard;
    auto velocity_camera = Vector4d{ 0.0, 0.0, 0.0, 0.0 };
    if (keyboard[SDL_SCANCODE_D]) {
        velocity_camera(0) += velocity;
    }
    if (keyboard[SDL_SCANCODE_A]) {
        velocity_camera(0) -= velocity;
    }
    if (keyboard[SDL_SCANCODE_W]) {
        velocity_camera(2) += velocity;
    }
    if (keyboard[SDL_SCANCODE_S]) {
        velocity_camera(2) -= velocity;
    }
    return velocity_camera;
}

Vector4d velocityInWorld(const Input& input, CameraExtrinsics extrinsics) {
    return worldFromCamera(extrinsics) * velocityInCamera(input);
}

CameraExtrinsics rotate(CameraExtrinsics extrinsics, const Input& input) {
    extrinsics.yaw += input.mouse_dx * angular_velocity;
    extrinsics.pitch -= input.mouse_dy * angular_velocity;
    extrinsics.pitch = clamp(extrinsics.pitch, -3.14 * 0.5, +3.14 * 0.5);
    return extrinsics;
}

void updateCameraBuildMode(Environment& environment, const Input& input) {
    auto& extrinsics = environment.extrinsics;
    const auto velocity_in_world = velocityInWorld(input, extrinsics);
    extrinsics.x += velocity_in_world.x();
    extrinsics.y += velocity_in_world.y();
    extrinsics.z += velocity_in_world.z();
    extrinsics = rotate(extrinsics, input);
}

int main(int, char**)
{
    const auto WINDOW_TITLE = "voxelot";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;

    auto pixels = Pixels(WIDTH, HEIGHT);
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);

    Environment environment = makeEnvironment(sdl.width, sdl.height);

    while (sdl.noQuitMessage()) {
        const auto input = sdl.getInput();
        if (input.escape_button == ButtonState::CLICKED) {
            return 0;
        }
        environment.timer.update();
        updateCameraBuildMode(environment, input);
        pixels.clear();
        drawBuildMode(pixels, environment);
        sdl.draw(pixels.colors.data());

    }
    return 0;
}
