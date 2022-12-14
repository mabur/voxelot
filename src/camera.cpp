#include "camera.hpp"

#include <Eigen/Geometry>

CameraIntrinsics makeCameraIntrinsics(size_t width, size_t height)
{
    auto intrinsics = CameraIntrinsics{};
    intrinsics.fx = 0.5 * height;
    intrinsics.fy = 0.5 * height;
    intrinsics.cx = 0.5 * width;
    intrinsics.cy = 0.5 * height;
    intrinsics.width = width;
    intrinsics.height = height;
    return intrinsics;
}

CameraExtrinsics makeCameraExtrinsics(const Vector4d& position)
{
    auto extrinsics = CameraExtrinsics{};
    extrinsics.x = position.x();
    extrinsics.y = position.y();
    extrinsics.z = position.z();
    extrinsics.yaw = 0.0;
    extrinsics.pitch = 0.0;
    return extrinsics;
}

Matrix4d worldFromCamera(const CameraExtrinsics& extrinsics)
{
    auto world_from_camera = Matrix4d{Matrix4d::Identity()};
    world_from_camera.col(3) << extrinsics.x, extrinsics.y, extrinsics.z, 1.0;
    using namespace Eigen;
    auto R = Matrix3d{};
    const auto R_flip  = AngleAxisd(3.14151965, Vector3d::UnitX());
    const auto R_yaw   = AngleAxisd(extrinsics.yaw, Vector3d::UnitY());
    const auto R_pitch = AngleAxisd(extrinsics.pitch, Vector3d::UnitX());
    R = R_flip * R_yaw * R_pitch;
    world_from_camera.topLeftCorner<3, 3>() = R;
    return world_from_camera;
}

Matrix4d imageFromCamera(const CameraIntrinsics& intrinsics)
{
    const auto& c = intrinsics;
    auto image_from_camera = Matrix4d{};
    image_from_camera <<
        c.fx, 0.0, c.cx, 0.0,
        0.0, c.fy, c.cy, 0.0,
        0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0;
    return image_from_camera;
}

Matrix4d imageFromWorld(
    const CameraIntrinsics& intrinsics,
    const CameraExtrinsics& extrinsics
) {
    return imageFromCamera(intrinsics) * worldFromCamera(extrinsics).inverse();
}

Vector4d cameraInWorld(const CameraExtrinsics& extrinsics) {
    return Vector4d{extrinsics.x, extrinsics.y, extrinsics.z, 1.0};
}
