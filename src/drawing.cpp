#include "drawing.hpp"

#include <algorithm>
#include <string>

#include <Eigen/Geometry>

#include "algorithm.hpp"
#include "color.hpp"
#include "map.hpp"
#include "text.hpp"
#include "vector_space.hpp"
#include "world.hpp"

namespace {

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

Matrix4d gridFromImage(const World& world) {
    const auto image_from_world = imageFromWorld(
        world.intrinsics, world.extrinsics
    );
    const auto world_from_grid = world.map.worldFromGrid();
    const auto image_from_grid = image_from_world * world_from_grid;
    return image_from_grid.inverse();
}

Vector4d normalizePosition(const Vector4d& v) {
    return v / v.w();
}

Vector4d normalizeDirection(const Vector4d& v) {
    return v / v.norm();
}

double startOffsetForAxis(double direction_in_grid, double p_in_grid) {
    return direction_in_grid > 0
        ? floor(p_in_grid) - p_in_grid
        : p_in_grid - ceil(p_in_grid);
}

const int* castSingleRay(
    const Vector4d& camera_in_grid,
    const Vector4d& direction_in_grid,
    const Voxels& voxels
) {
    const auto grid_width = static_cast<double>(voxels.width());
    const auto grid_height = static_cast<double>(voxels.height());
    const auto grid_depth = static_cast<double>(voxels.depth());

    const Vector4d dx = direction_in_grid / std::abs(direction_in_grid.x());
    const Vector4d dy = direction_in_grid / std::abs(direction_in_grid.y());
    const Vector4d dz = direction_in_grid / std::abs(direction_in_grid.z());

    const double start_offset_x = startOffsetForAxis(
        direction_in_grid.x(), camera_in_grid.x()
    );
    const double start_offset_y = startOffsetForAxis(
        direction_in_grid.y(), camera_in_grid.y()
    );
    const double start_offset_z = startOffsetForAxis(
        direction_in_grid.z(), camera_in_grid.z()
    );

    Vector4d px = camera_in_grid + start_offset_x * dx;
    Vector4d py = camera_in_grid + start_offset_y * dy;
    Vector4d pz = camera_in_grid + start_offset_z * dz;

    double dtx = dx.norm();
    double dty = dy.norm();
    double dtz = dz.norm();

    double tx = start_offset_x * dtx;
    double ty = start_offset_y * dty;
    double tz = start_offset_z * dtz;

    for (;;) {
        auto xg = 0.0;
        auto yg = 0.0;
        auto zg = 0.0;
        if (tx <= ty && tx <= tz) {
            xg = floor(px.x());
            yg = floor(px.y());
            zg = floor(px.z());
            if (direction_in_grid.x() < 0) xg--;
            px += dx;
            tx += dtx;
        }
        else if (ty <= tx && ty <= tz) {
            xg = floor(py.x());
            yg = floor(py.y());
            zg = floor(py.z());
            if (direction_in_grid.y() < 0) yg--;
            py += dy;
            ty += dty;
        }
        else {
            xg = floor(pz.x());
            yg = floor(pz.y());
            zg = floor(pz.z());
            if (direction_in_grid.z() < 0) zg--;
            pz += dz;
            tz += dtz;
        }

        if (direction_in_grid.x() < 0 && xg < 0) return nullptr;
        if (direction_in_grid.y() < 0 && yg < 0) return nullptr;
        if (direction_in_grid.z() < 0 && zg < 0) return nullptr;
        if (direction_in_grid.x() > 0 && grid_width - 1 < xg) return nullptr;
        if (direction_in_grid.y() > 0 && grid_height - 1 < yg) return nullptr;
        if (direction_in_grid.z() > 0 && grid_depth - 1 < zg) return nullptr;

        if (0 <= xg && xg <= grid_width - 1 &&
            0 <= yg && yg <= grid_height - 1 &&
            0 <= zg && zg <= grid_depth - 1) {
            const auto xgi = static_cast<size_t>(xg);
            const auto ygi = static_cast<size_t>(yg);
            const auto zgi = static_cast<size_t>(zg);
            if (voxels(xgi, ygi, zgi)) {
                return &voxels(xgi, ygi, zgi);
            }
        }
    }
}

void rayCastVoxelsPrecise(Pixels& pixels, const World& world) {
    const auto grid_from_image = gridFromImage(world);
    const auto grid_from_world = world.map.gridFromWorld();
    const auto camera_in_world = cameraInWorld(world.extrinsics);
    const auto camera_in_grid = normalizePosition(
        grid_from_world * camera_in_world
    );
    double yd = 0.0;
    for (size_t y = 0; y < pixels.height(); ++y, ++yd) {
        double xd = 0.0;
        for (size_t x = 0; x < pixels.width(); ++x, ++xd) {
            const auto p_in_image = Vector4d{xd, yd, 1, 1};
            const auto p_in_grid = normalizePosition(
                grid_from_image * p_in_image
            );
            const auto direction_in_grid = (p_in_grid - camera_in_grid).eval();
            const auto voxel_pointer = castSingleRay(
                camera_in_grid, direction_in_grid, world.map.voxels
            );
            if (voxel_pointer) {
                pixels(x, y) = RED;
            }
        }
    }
}

void rayCastVoxels(Pixels& pixels, const World& world) {
    const auto& voxels = world.map.voxels;

    const auto image_width = static_cast<double>(pixels.width());
    const auto image_height = static_cast<double>(pixels.height());

    const auto grid_width = static_cast<double>(voxels.width());
    const auto grid_height = static_cast<double>(voxels.height());
    const auto grid_depth = static_cast<double>(voxels.depth());

    const auto grid_from_image = gridFromImage(world);
    const auto step_length = 0.1;
    const auto num_steps = 100;

    const auto grid_from_world = world.map.gridFromWorld();
    const auto camera_in_grid = normalizePosition(
        grid_from_world * Vector4d{
        world.extrinsics.x,
        world.extrinsics.y,
        world.extrinsics.z,
        1.0
    });

    for (auto y = 0.0; y < image_height; ++y) {
        for (auto x = 0.0; x < image_width; ++x) {
            auto p_in_grid = normalizePosition(
                grid_from_image * Vector4d{x, y, 1, 1}
            );
            const auto offset = step_length * normalizeDirection(
                p_in_grid - camera_in_grid
            );
            for (auto i = 0; i < num_steps; ++i, p_in_grid += offset) {
                const auto xg = std::floor(p_in_grid.x());
                const auto yg = std::floor(p_in_grid.y());
                const auto zg = std::floor(p_in_grid.z());
                if (0 <= xg && xg <= grid_width - 1 &&
                    0 <= yg && yg <= grid_height - 1 &&
                    0 <= zg && zg <= grid_depth - 1) {
                    const auto xgi = static_cast<size_t>(xg);
                    const auto ygi = static_cast<size_t>(yg);
                    const auto zgi = static_cast<size_t>(zg);
                    if (voxels(xgi, ygi, zgi)) {
                        const auto xi = static_cast<size_t>(x);
                        const auto yi = static_cast<size_t>(y);
                        pixels(xi, yi) = RED;
                        break;
                    }
                }
            }
        }
    }
}

}; // namespace

void draw(Pixels& pixels, const World& world) {

    const auto width = world.map.voxels.width();
    const auto height = world.map.voxels.height();
    const auto depth = world.map.voxels.depth();

    fill(pixels, BLACK);

    // rayCastVoxels(pixels, world);
    rayCastVoxelsPrecise(pixels, world);

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
                    drawPoints(pixels, image_from_world, points, GRAY);
                }
            }
        }
    }
    drawPoints(pixels, image_from_world, activeBlockMesh(world), GREEN);
    drawFrameFrequency(pixels, world);
}
