#pragma once

#include "button.hpp"

enum class ControlMode { GUI_2D, VOXEL_3D };
enum class BuildMode { BUILD_BLOCK, BUILD_SCENE };

struct Gui {
    SingleSelectButtons<int> building_block_buttons;
    SingleSelectButtons<BuildMode> build_mode_buttons;
};

Gui makeGui(int screen_width, int screen_height);
