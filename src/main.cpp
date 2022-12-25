#define SDL_MAIN_HANDLED

#include "algorithm.hpp"
#include "button.hpp"
#include "camera.hpp"
#include "drawing.hpp"
#include "sdl_wrappers.hpp"
#include "world.hpp"

enum class ControlMode {GUI_2D, VOXEL_3D};

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

void updateMap(World& world, const Input& input) {
    const auto voxel_in_grid = selectedVoxel(world);
    const auto x = voxel_in_grid.x();
    const auto y = voxel_in_grid.y();
    const auto z = voxel_in_grid.z();
    if (input.isLeftMouseButtonDown()) {
        world.map.voxels(x, y, z) = 1;
    }
    if (input.isRightMouseButtonDown()) {
        world.map.voxels(x, y, z) = 0;
    }
}

using BuildingBlockButtons = SingleSelectButtons<int>;

BuildingBlockButtons makeBuildingBlockButtons(
    int screen_width, int screen_height
) {
    auto buttons = BuildingBlockButtons{};
    for (auto i = 0; i < 8; ++i) {
        Button button;
        button.text = std::to_string(i);
        button.rectangle.w = 16;
        button.rectangle.h = 16;
        button.rectangle.x = 0 + i * button.rectangle.w;
        button.rectangle.y = 0;
        button.r = 60;
        button.g = 180;
        button.b = 75;
        buttons[i] = button;
    }
    return buttons;
}

int main(int, char**)
{
    const auto WINDOW_TITLE = "voxelot";
    const auto WIDTH = 320;
    const auto HEIGHT = 200;

    auto pixels = Pixels(WIDTH, HEIGHT, BLACK);
    auto sdl = Sdl(WINDOW_TITLE, WIDTH, HEIGHT);
    auto world = makeWorld(sdl.width, sdl.height);
    auto building_block_buttons = makeBuildingBlockButtons(WIDTH, HEIGHT);
    auto control_mode = ControlMode::VOXEL_3D;

    while (true) {
        auto e = SDL_Event();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                const auto key = e.key.keysym.sym;
                if (key == SDLK_ESCAPE) {
                    return 0;
                }
                if (key == SDLK_SPACE) {
                    if (control_mode == ControlMode::VOXEL_3D) {
                        control_mode = ControlMode::GUI_2D;
                        sdl.setMouseModeAbsolute();
                    }
                    else {
                        control_mode = ControlMode::VOXEL_3D;
                        sdl.setMouseModeRelative();
                    }
                }
            }
        }
        const auto input = sdl.getInput();
        world.timer.update();
        if (control_mode == ControlMode::VOXEL_3D) {
            world.extrinsics = updateCamera(world.extrinsics, input);
            updateMap(world, input);
        }
        else {
            if (input.isLeftMouseButtonDown()) {
                handleMouseDown(
                    SDL_Point{input.mouse_x, input.mouse_y},
                    building_block_buttons
                );
            }
        }
        draw(pixels, world);
        for (auto item : building_block_buttons) {
            drawButton(item.second, pixels);
        }
        sdl.draw(pixels.data());
        SDL_RenderPresent(sdl.renderer);
    }
    return 0;
}
