#pragma once

#include "vector_space.hpp"

struct CameraIntrinsics
{
    double fx;
    double fy;
    double cx;
    double cy;
    size_t width;
    size_t height;
};

struct CameraExtrinsics
{
    double x;
    double y;
    double z;
    double yaw;
    double pitch;
};

CameraIntrinsics makeCameraIntrinsics(size_t width, size_t height);
CameraExtrinsics makeCameraExtrinsics(const Vector4d& position);

Matrix4d worldFromCamera(const CameraExtrinsics& coordinates);

Matrix4d imageFromWorld(
    const CameraIntrinsics& intrinsics,
    const CameraExtrinsics& extrinsics
);
