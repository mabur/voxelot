#pragma once

#include "button.hpp"

enum class ControlMode { GUI_2D, VOXEL_3D };
enum class BuildMode { BUILD_BLOCK, BUILD_SCENE };

using BuildingBlockButtons = SingleSelectButtons<int>;
using BuildModeButtons = SingleSelectButtons<BuildMode>;

BuildingBlockButtons makeBuildingBlockButtons(
    int screen_width, int screen_height
);

BuildModeButtons makeBuildModeButtons(
    int screen_width, int screen_height
);
