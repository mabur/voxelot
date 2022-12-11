#pragma once

#include <Eigen/Core>
#include <Eigen/StdVector>

using Vector4d = Eigen::Vector4d;
using Vector4s = Eigen::Matrix<size_t, 4, 1>;
using Matrix4d = Eigen::Matrix4d;
using Vectors4d = std::vector<Vector4d, Eigen::aligned_allocator<Vector4d>>;
