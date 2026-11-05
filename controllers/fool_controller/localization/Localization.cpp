#include "Localization.h"

Localization::Localization() : _pose(), _startPose(), _hasStartPose(false) {}

Pose2D Localization::update(const SensorSnapshot &sensors) {
  // TODO: fuse GPS, compass and wheel odometry; capture the initial pose.
  (void)sensors;
  return _pose;
}

Pose2D Localization::startPose() const {
  return _startPose;
}

bool Localization::hasStartPose() const {
  return _hasStartPose;
}
