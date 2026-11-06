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
// All 16 devices are lasers. The front six retain the practice-03 laser curve;
// ds3/ds12 keep 1 m tables, and all other sensors keep 0.4 m tables.
constexpr double kFrontLaserProfileSaturationResponse = 1020.0;
constexpr double kOtherLaserProfilesSaturationResponse = 1024.0;
constexpr double kFrontLaserProfileNearSaturationM = 0.08;
constexpr double kOtherLaserProfilesNearSaturationM = 0.2;
constexpr double kDistanceSensorLongRangeM = 1.0;
constexpr double kDistanceSensorShortRangeM = 0.4;
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

constexpr bool isLongRangeDistanceSensor(int index) {
  return index == 3 || index == 12;
}

constexpr bool usesFrontLaserLookupProfile(int index) {
  return (index >= 0 && index <= 2) || (index >= 13 && index <= 15);
}

constexpr double distanceSensorMaximumRangeM(int index) {
  return isLongRangeDistanceSensor(index) ? kDistanceSensorLongRangeM
                                          : kDistanceSensorShortRangeM;
}

constexpr double distanceSensorNearSaturationM(int index) {
  return usesFrontLaserLookupProfile(index)
             ? kFrontLaserProfileNearSaturationM
             : kOtherLaserProfilesNearSaturationM;
}

constexpr double distanceSensorSaturationResponse(int index) {
  return usesFrontLaserLookupProfile(index)
             ? kFrontLaserProfileSaturationResponse
             : kOtherLaserProfilesSaturationResponse;
}

// Raw DistanceSensor thresholds ported from practica03/obstacle_avoidance.
constexpr double kFrontWarningThresholdRaw = 100.0;
constexpr double kFrontObstacleThresholdRaw = 190.0;
constexpr double kFrontClearThresholdRaw = 100.0;
constexpr double kSideDangerThresholdRaw = 850.0;
constexpr double kWallTargetRaw = 570.0;
constexpr double kWallDetectThresholdRaw = 300.0;
constexpr double kWallLostThresholdRaw = 180.0;

// Initial values; tune from repeated map-identification trials.
constexpr double kMapMatchConfidenceThreshold = 0.85;
constexpr double kMapMatchMinimumMargin = 0.10;

}  // namespace robot_config

#endif  // RESCUE_ROBOT_CONFIG_H_
