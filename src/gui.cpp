#include "gui.hpp"

#include "draw_gui.hpp"

SingleSelectButtons<int> makeBuildingBlockButtons(
    int screen_width, int screen_height
) {
    const auto NUM_BUTTONS = 8;
    const auto BUTTON_SIZE = 16;
    const auto START_X = screen_width - NUM_BUTTONS * BUTTON_SIZE;
    auto buttons = SingleSelectButtons<int>{};
    for (auto i = 0; i < NUM_BUTTONS; ++i) {
        auto button = Button{};
        button.text = std::to_string(i + 1);
        button.rectangle.w = BUTTON_SIZE;
        button.rectangle.h = BUTTON_SIZE;
        button.rectangle.x = START_X + i * BUTTON_SIZE;
        button.rectangle.y = 0;
        button.color = Color{ 60, 180, 75 };
        buttons[i] = button;
    }
    buttons[0].selected = true;
    return buttons;
}

SingleSelectButtons<BuildMode> makeBuildModeButtons(
    int screen_width, int screen_height
) {
    const auto BUTTON_WIDTH = 48;
    const auto BUTTON_HEIGHT = 16;
    const auto START_X = 0;

    const auto COLOR = Color{ 64, 128, 255 };

    auto buttons = SingleSelectButtons<BuildMode>{};

    auto button_block = Button{};
    button_block.text = "Block";
    button_block.rectangle.w = BUTTON_WIDTH;
    button_block.rectangle.h = BUTTON_HEIGHT;
    button_block.rectangle.x = START_X + 0 * BUTTON_WIDTH;
    button_block.rectangle.y = 0;
    button_block.color = COLOR;
    button_block.selected = true;

    auto button_scene = Button{};
    button_scene.text = "Scene";
    button_scene.rectangle.w = BUTTON_WIDTH;
    button_scene.rectangle.h = BUTTON_HEIGHT;
    button_scene.rectangle.x = START_X + 1 * BUTTON_WIDTH;
    button_scene.rectangle.y = 0;
    button_scene.color = COLOR;

    buttons[BuildMode::BUILD_BLOCK] = button_block;
    buttons[BuildMode::BUILD_SCENE] = button_scene;

    return buttons;
}

Gui makeGui(int screen_width, int screen_height) {
    auto gui = Gui{};
    gui.building_block_buttons = makeBuildingBlockButtons(screen_width, screen_height);
    gui.build_mode_buttons = makeBuildModeButtons(screen_width, screen_height);
    return gui;
}

void handleMouseDown(SDL_Point mouse, Gui& gui) {
    handleMouseDown(mouse, gui.building_block_buttons);
    handleMouseDown(mouse, gui.build_mode_buttons);
}

void drawGui(Pixels& pixels, const Gui& gui) {
    drawButtons(pixels, gui.building_block_buttons);
    drawButtons(pixels, gui.build_mode_buttons);
}
