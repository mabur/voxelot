#include "mesh.hpp"

#include <string>
#include <vector>

#include "block.hpp"
#include "map.hpp"

Mesh buildAabb(Vector4d block_center) {
    const auto d = 0.5 * L;
    return Mesh{ {
        block_center + Vector4d{ -d, -d, +d, 0 },
        block_center + Vector4d{ +d, -d, +d, 0 },
        block_center + Vector4d{ +d, +d, +d, 0 },
        block_center + Vector4d{ -d, +d, +d, 0 },
        block_center + Vector4d{ -d, -d, -d, 0 },
        block_center + Vector4d{ +d, -d, -d, 0 },
        block_center + Vector4d{ +d, +d, -d, 0 },
        block_center + Vector4d{ -d, +d, -d, 0 },
    } };
}
