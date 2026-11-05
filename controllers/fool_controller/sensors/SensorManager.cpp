#include "SensorManager.h"

SensorManager::SensorManager(webots::Robot *robot)
    : _robot(robot), _timeStep(0) {}

void SensorManager::initialize(int timeStep) {
  _timeStep = timeStep;
  (void)_robot;
  // TODO: acquire and enable the distance sensors, encoders, GPS, compass,
  // and cameras required by the selected behaviors.
}

void SensorManager::shutdown() {
  // TODO: disable enabled devices during controller shutdown.
}

SensorSnapshot SensorManager::read() const {
  // TODO: convert device readings to SI units and apply noise filtering.
  return SensorSnapshot();
}
