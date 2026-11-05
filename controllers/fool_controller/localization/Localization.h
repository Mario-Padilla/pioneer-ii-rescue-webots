#ifndef RESCUE_LOCALIZATION_H_
#define RESCUE_LOCALIZATION_H_

#include "../sensors/SensorSnapshot.h"

struct Pose2D {
  Pose2D() : xM(0.0), zM(0.0), headingRad(0.0) {}

  double xM;
  double zM;
  double headingRad;
};

class Localization {
public:
  Localization();

  Pose2D update(const SensorSnapshot &sensors);
  Pose2D startPose() const;
  bool hasStartPose() const;

private:
  Pose2D _pose;
  Pose2D _startPose;
  bool _hasStartPose;
};

#endif  // RESCUE_LOCALIZATION_H_
