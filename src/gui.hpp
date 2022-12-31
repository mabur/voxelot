#pragma once

#include "button.hpp"
#include "text_label.hpp"
#include "pixels.hpp"

enum class BuildingBlock { B1, B2, B3, B4, B5, B6, B7, B8, COUNT };
enum class BuildMode { BUILD_BLOCK, BUILD_SCENE };

struct Gui {
    SingleSelectButtons<BuildingBlock> building_block_buttons;
    SingleSelectButtons<BuildMode> build_mode_buttons;
    TextLabel frame_frequency;
};

Gui makeGui(int screen_width, int screen_height);
void handleMouseDown(SDL_Point mouse, Gui& gui);
void drawGui(Pixels& pixels, const Gui& gui);
