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

Vectors4d getCubeCorners(const Vector4d& center, double side) {
    const auto d = 0.5 * side;
    return {
        center + Vector4d{ -d, -d, +d, 0 },
        center + Vector4d{ +d, -d, +d, 0 },
        center + Vector4d{ +d, +d, +d, 0 },
        center + Vector4d{ -d, +d, +d, 0 },
        center + Vector4d{ -d, -d, -d, 0 },
        center + Vector4d{ +d, -d, -d, 0 },
        center + Vector4d{ +d, +d, -d, 0 },
        center + Vector4d{ -d, +d, -d, 0 },
    };
}

Vectors4d activeBlockMesh(const World& world) {
    const auto& map = world.map;
    const auto camera_view_position = 
        (worldFromCamera(world.extrinsics) *
        Vector4d { 0, 0, 2 * map.voxel_length, 1 }).eval();
    const auto block_center = map.closestVoxelCenterInWorld(
        camera_view_position
    );
    return getCubeCorners(block_center, map.voxel_length);
}

void drawPoints(
    Pixels& pixels,
    const Matrix4d& image_from_world,
    const Vectors4d& positions_in_world,
    Color color
) {
    for (const auto& position_world : positions_in_world) {
        const auto position_image = (image_from_world * position_world).eval();
        const auto p = position_image / position_image(3);
        drawPoint(pixels, p, color);
    }
}

void drawBuildMode(Pixels& pixels, const World& world) {
    const auto image_from_world = imageFromWorld(
        world.intrinsics, world.extrinsics
    );

    const auto d = world.map.voxel_length;
    size_t i = 0;
    for (size_t z = 0; z < world.map.voxels.depth(); ++z) {
        for (size_t y = 0; y < world.map.voxels.height(); ++y) {
            for (size_t x = 0; x < world.map.voxels.width(); ++x) {
                const auto points = Vectors4d{
                    Vector4d{d * x, d * y, d * z, 1.0}
                };
                drawPoints(pixels, image_from_world, points, GRAY);
                ++i;
            }
        }
    }

    drawPoints(pixels, image_from_world, activeBlockMesh(world), GREEN);
    drawFrameFrequency(pixels, world);
}
