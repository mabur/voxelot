#include "draw.hpp"

#include <algorithm>
#include <string>

#include <Eigen/Geometry>

#include "algorithm.hpp"
#include "color.hpp"
#include "draw_gui.hpp"
#include "draw_voxels.hpp"
#include "map.hpp"
#include "text.hpp"
#include "text_label.hpp"
#include "vector_space.hpp"
#include "world.hpp"

namespace {

void drawFrameFrequency(Pixels& pixels, const World& world) {
    auto label = TextLabel{};
    label.text = std::to_string(world.timer.frequency()) + "Hz";
    label.color = WHITE;
    label.x = static_cast<int>(pixels.width()) - 8 * 5;
    label.y = static_cast<int>(pixels.height()) - 16;
    drawTextLabel(pixels, label);
}

bool isBehindCamera(const Vector4d& v)
{
    return v(2) <= 0;
}

void drawPoint(Pixels& pixels, Vector4d position_image, PackedColor color) {
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
    const auto voxel_in_grid = selectedVoxel(world);
    const auto block_center = map.voxelCenterInWorld(voxel_in_grid);
    return getCubeCorners(block_center, map.voxel_length);
}

void drawPoints(
    Pixels& pixels,
    const Matrix4d& image_from_world,
    const Vectors4d& positions_in_world,
    PackedColor color
) {
    for (const auto& position_world : positions_in_world) {
        const auto position_image = (image_from_world * position_world).eval();
        const auto p = position_image / position_image(3);
        drawPoint(pixels, p, color);
    }
}

}; // namespace

void draw(Pixels& pixels, const World& world) {

    const auto width = world.map.voxels.width();
    const auto height = world.map.voxels.height();
    const auto depth = world.map.voxels.depth();

    fill(pixels, packColor(GRAY128));

    const auto image_from_world = imageFromWorld(
        world.intrinsics, world.extrinsics
    );
    const auto world_from_grid = world.map.worldFromGrid();
    const auto d = world.map.voxel_length;
    for (size_t z = 0; z < depth + 1; ++z) {
        for (size_t y = 0; y < height + 1; ++y) {
            for (size_t x = 0; x < width + 1; ++x) {
                const auto points = Vectors4d{
                    world_from_grid * Vector4s{x, y, z, 1}.cast<double>(),
                };
                if (x == 0 or y == 0 or z == 0 or x == width or y == height or z == depth) {
                    drawPoints(pixels, image_from_world, points, packColor(GRAY192));
                }
            }
        }
    }
    rayCastVoxels(pixels, world);
    drawPoints(pixels, image_from_world, activeBlockMesh(world), packColor(GREEN));
    drawFrameFrequency(pixels, world);
}
