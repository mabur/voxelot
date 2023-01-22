#include "draw_voxels.hpp"

#include <algorithm>

#include <Eigen/Geometry>

#include "algorithm.hpp"
#include "color.hpp"
#include "map.hpp"
#include "vector_space.hpp"
#include "world.hpp"

namespace {

Matrix4d gridFromImage(const World& world) {
    const auto image_from_world = imageFromWorld(
        world.intrinsics, world.extrinsics
    );
    const auto& map = world.building_blocks.at(world.active_building_block);
    const auto world_from_grid = map.worldFromGrid();
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
        ? ceil(p_in_grid) - p_in_grid
        : p_in_grid - floor(p_in_grid);
}

enum class Direction {
    X_POSITIVE, X_NEGATIVE, Y_POSITIVE, Y_NEGATIVE, Z_POSITIVE, Z_NEGATIVE
};

struct Intersection {
    size_t x = 0;
    size_t y = 0;
    size_t z = 0;
    Direction ray_direction = Direction::X_POSITIVE;
    bool did_hit = false;
};

Intersection castSingleRay(
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

    const double dtx = dx.norm();
    const double dty = dy.norm();
    const double dtz = dz.norm();

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

    double tx = start_offset_x * dtx;
    double ty = start_offset_y * dty;
    double tz = start_offset_z * dtz;

    const auto x_direction = direction_in_grid.x() < 0 ?
        Direction::X_NEGATIVE : Direction::X_POSITIVE;
    const auto y_direction = direction_in_grid.y() < 0 ?
        Direction::Y_NEGATIVE : Direction::Y_POSITIVE;
    const auto z_direction = direction_in_grid.z() < 0 ?
        Direction::Z_NEGATIVE : Direction::Z_POSITIVE;

    for (;;) {
        auto ray_direction = Direction{};

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
            ray_direction = x_direction;
        }
        else if (ty <= tx && ty <= tz) {
            xg = floor(py.x());
            yg = floor(py.y());
            zg = floor(py.z());
            if (direction_in_grid.y() < 0) yg--;
            py += dy;
            ty += dty;
            ray_direction = y_direction;
        }
        else {
            xg = floor(pz.x());
            yg = floor(pz.y());
            zg = floor(pz.z());
            if (direction_in_grid.z() < 0) zg--;
            pz += dz;
            tz += dtz;
            ray_direction = z_direction;
        }

        if (direction_in_grid.x() < 0 && xg < 0) return {};
        if (direction_in_grid.y() < 0 && yg < 0) return {};
        if (direction_in_grid.z() < 0 && zg < 0) return {};
        if (direction_in_grid.x() > 0 && grid_width - 1 < xg) return {};
        if (direction_in_grid.y() > 0 && grid_height - 1 < yg) return {};
        if (direction_in_grid.z() > 0 && grid_depth - 1 < zg) return {};

        if (0 <= xg && xg <= grid_width - 1 &&
            0 <= yg && yg <= grid_height - 1 &&
            0 <= zg && zg <= grid_depth - 1) {
            const auto xgi = static_cast<size_t>(xg);
            const auto ygi = static_cast<size_t>(yg);
            const auto zgi = static_cast<size_t>(zg);
            if (voxels(xgi, ygi, zgi)) {
                return Intersection{xgi, ygi, zgi, ray_direction, true};
            }
        }
    }
}

PackedColor colorFromRayDirection(Direction d) {
    switch (d) {
        case Direction::X_POSITIVE: return packColor(Color{255,64,64});
        case Direction::X_NEGATIVE: return packColor(Color{192,0,0});
        case Direction::Y_POSITIVE: return packColor(Color{128,0,0});
        case Direction::Y_NEGATIVE: return packColor(Color{255,128,128});
        case Direction::Z_POSITIVE: return packColor(Color{255,0,0});
        case Direction::Z_NEGATIVE: return packColor(Color{255,0,0});
    }
}

}; // namespace

void rayCastVoxels(Pixels& pixels, const World& world) {
    const auto& map = world.building_blocks.at(world.active_building_block);
    const auto grid_from_image = gridFromImage(world);
    const auto grid_from_world = map.gridFromWorld();
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
            const auto direction_in_grid = normalizeDirection(p_in_grid - camera_in_grid);
            const auto intersection = castSingleRay(
                camera_in_grid, direction_in_grid, map.voxels
            );
            if (intersection.did_hit) {
                const auto color = colorFromRayDirection(intersection.ray_direction);
                pixels(x, y) = color;
            }
        }
    }
}
