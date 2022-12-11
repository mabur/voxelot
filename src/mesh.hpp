#pragma once

#include <vector>

#include "vector_space.hpp"

struct Map;

struct Mesh {
    Vectors4d positions_world;
};

Mesh buildAabb(Vector4d block_center);
