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

Vectors4d selectedVoxelGridPoints(const World& world) {
    const auto& map = world.building_blocks.at(world.active_building_block);
    const auto voxel_in_grid = selectedVoxel(world);
    const auto block_center = map.voxelCenterInWorld(voxel_in_grid);
    return getCubeCorners(block_center, map.voxel_length);
}

Vectors4d voxelGridPoints(const World& world) {
    const auto& map = world.building_blocks.at(world.active_building_block);
    const auto world_from_grid = map.worldFromGrid();
    const auto width = map.voxels.width();
    const auto height = map.voxels.height();
    const auto depth = map.voxels.depth();
    auto points = Vectors4d{};
    for (size_t z = 0; z < depth + 1; ++z) {
        for (size_t y = 0; y < height + 1; ++y) {
            for (size_t x = 0; x < width + 1; ++x) {
                if (x == 0 or y == 0 or z == 0 or x == width or y == height or z == depth) {
                    points.push_back(world_from_grid * Vector4s{ x, y, z, 1 }.cast<double>());
                }
            }
        }
    }
    return points;
}

void drawPoints(
    Pixels& pixels,
    const Matrix4d& image_from_world,
    const Vectors4d& positions_in_world,
    Color color
) {
    const auto packed_color = packColor(color);
    for (const auto& position_world : positions_in_world) {
        const auto position_image = (image_from_world * position_world).eval();
        const auto p = position_image / position_image(3);
        drawPoint(pixels, p, packed_color);
    }
}

}; // namespace

void draw3d(Pixels& pixels, const World& world) {
    const auto image_from_world = imageFromWorld(
        world.intrinsics, world.extrinsics
    );
    drawPoints(pixels, image_from_world, voxelGridPoints(world), GRAY192);
    rayCastVoxels(pixels, world);
    drawPoints(pixels, image_from_world, selectedVoxelGridPoints(world), GREEN);
}
