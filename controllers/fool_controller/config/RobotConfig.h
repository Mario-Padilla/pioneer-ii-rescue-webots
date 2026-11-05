#ifndef RESCUE_ROBOT_CONFIG_H_
#define RESCUE_ROBOT_CONFIG_H_

namespace robot_config {

// Webots device names from the Pioneer II PROTO.
constexpr const char *kLeftMotorName = "left wheel motor";
constexpr const char *kRightMotorName = "right wheel motor";
constexpr const char *kLeftEncoderName = "left wheel sensor";
constexpr const char *kRightEncoderName = "right wheel sensor";
constexpr const char *kCompassName = "compass";
constexpr const char *kGpsName = "gps";
constexpr const char *kFrontCameraName = "camera_f";
constexpr const char *kSphericalCameraName = "camera_s";

constexpr int kDistanceSensorCount = 16;
constexpr double kMaximumWheelSpeedRadPerSec = 10.0;
constexpr double kVictimRescueDistanceM = 1.0;
constexpr double kWheelRadiusM = 0.0825;
constexpr double kWheelSeparationM = 0.32;

// Static maps use world X/Z coordinates (Y is vertical in Webots).
constexpr double kMapOriginXM = -5.0;
constexpr double kMapOriginZM = -10.0;
constexpr double kMapWidthM = 10.0;
constexpr double kMapLengthM = 20.0;
constexpr double kMapCellSizeM = 0.2;

// Initial values; tune from repeated map-identification trials.
constexpr double kMapMatchConfidenceThreshold = 0.85;
constexpr double kMapMatchMinimumMargin = 0.10;

}  // namespace robot_config

#endif  // RESCUE_ROBOT_CONFIG_H_
