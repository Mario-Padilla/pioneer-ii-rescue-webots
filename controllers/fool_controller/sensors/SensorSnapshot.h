#ifndef RESCUE_SENSOR_SNAPSHOT_H_
#define RESCUE_SENSOR_SNAPSHOT_H_

#include <array>

#include "../config/RobotConfig.h"

struct SensorSnapshot {
  SensorSnapshot()
      : gpsXM(0.0),
        gpsZM(0.0),
        compassHeadingRad(0.0),
        leftEncoderRad(0.0),
        rightEncoderRad(0.0),
        gpsValid(false),
        compassValid(false),
        encodersValid(false),
        distanceSensorsValid(false),
        distanceMeters() {}

  double gpsXM;
  double gpsZM;
  double compassHeadingRad;
  double leftEncoderRad;
  double rightEncoderRad;
  bool gpsValid;
  bool compassValid;
  bool encodersValid;
  bool distanceSensorsValid;
  std::array<double, robot_config::kDistanceSensorCount> distanceMeters;
};

#endif  // RESCUE_SENSOR_SNAPSHOT_H_
