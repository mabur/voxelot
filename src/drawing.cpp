#include "drawing.hpp"

#include <algorithm>
#include <string>

#include <Eigen/Core>

#include "algorithm.hpp"
#include "color.hpp"
#include "map.hpp"
#include "text.hpp"
#include "vector_space.hpp"
#include "world.hpp"

void drawCharacter(
    Pixels& pixels, char character, size_t x_start, size_t y_start, Color color
) {
    const auto& digit_bitmap = text_bitmaps::get(character);
    for (size_t y = 0; y < 8; ++y) {
        for (size_t x = 0; x < 8; ++x) {
            if (digit_bitmap[y * 8 + x]) {
                pixels(x_start + x, y_start + y) = color;
            }
        }
    }
}

void drawString(
    Pixels& pixels, const std::string& s, size_t x, size_t y, Color color
) {
    for (size_t i = 0; i < s.size(); ++i) {
        drawCharacter(pixels, s[i], x + 8 * i, y, color);
    }
}

void drawFrameFrequency(Pixels& pixels, const World& world) {
    const auto time = std::to_string(world.timer.frequency()) + "Hz";
    const auto x = pixels.width() - 8 * 5;
    const auto y = pixels.height() - 16;
    drawString(pixels, time, x, y, WHITE);
}

bool isBehindCamera(const Vector4d& v)
{
    return v(2) <= 0;
}

void drawPoint(Pixels& pixels, Vector4d position_image, Color color) {
    const auto width = pixels.width();
    const auto height = pixels.height();
    if (isBehindCamera(position_image)) {
        return;
    }
    const auto x = roundDoubleToInt(position_image.x());
    const auto y = roundDoubleToInt(position_image.y());
    if (x < 0 || width <= x || y < 0 || height <= y) {
        return;
    }
    pixels(x, y) = color;
}

Vectors4d activeBlockMesh(const World& world) {
    const auto& map = world.map;
    const auto camera_view_position = 
        (worldFromCamera(world.extrinsics) *
        Vector4d { 0, 0, 2 * world.map.voxel_length, 1 }).eval();
    const auto block_center = map.closestVoxelCenterInWorld(
        camera_view_position
    );
    const auto d = 0.5 * world.map.voxel_length;
    return {
        block_center + Vector4d{ -d, -d, +d, 0 },
        block_center + Vector4d{ +d, -d, +d, 0 },
        block_center + Vector4d{ +d, +d, +d, 0 },
        block_center + Vector4d{ -d, +d, +d, 0 },
        block_center + Vector4d{ -d, -d, -d, 0 },
        block_center + Vector4d{ +d, -d, -d, 0 },
        block_center + Vector4d{ +d, +d, -d, 0 },
        block_center + Vector4d{ -d, +d, -d, 0 },
    };
}

void drawBuildMode(Pixels& pixels, const World& world) {
    static Vectors4d positions_world;
    static Vectors4d positions_image;

    positions_world.clear();
    positions_image.clear();
    positions_world = activeBlockMesh(world);
    
    const auto image_from_world = imageFromWorld(
        world.intrinsics, world.extrinsics
    );
    for (const auto& position_world : positions_world) {
        const auto position_image = (image_from_world * position_world).eval(); 
        positions_image.push_back(position_image / position_image(3));
    }
    for (const auto& p : positions_image) {
        drawPoint(pixels, p, GREEN);
    }
    drawFrameFrequency(pixels, world);
}
