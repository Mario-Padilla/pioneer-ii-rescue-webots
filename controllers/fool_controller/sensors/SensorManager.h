#ifndef RESCUE_SENSOR_MANAGER_H_
#define RESCUE_SENSOR_MANAGER_H_

#include <webots/Robot.hpp>

#include "SensorSnapshot.h"

class SensorManager {
public:
  explicit SensorManager(webots::Robot *robot);

  void initialize(int timeStep);
  void shutdown();
  SensorSnapshot read() const;

private:
  webots::Robot *_robot;
  int _timeStep;
};

#endif  // RESCUE_SENSOR_MANAGER_H_
