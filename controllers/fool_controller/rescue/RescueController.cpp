#include "RescueController.h"

RescueController::RescueController()
    : _sensors(&_robot),
      _localization(),
      _mapCatalog(),
      _mapMatcher(),
      _planner(),
      _navigator(),
      _missionManager(),
      _motorController(&_robot) {}

void RescueController::run() {
  const int timeStep = static_cast<int>(_robot.getBasicTimeStep());

  // Safe scaffold: the planned flow is to identify the static map while
  // moving safely, plan from the current pose, and follow the route.
  _motorController.initialize();
  _sensors.initialize(timeStep);

  while (_robot.step(timeStep) != -1) {
    // TODO: snapshot -> localization -> progressive map matching -> mission.
    // Once matched: A* route -> reactive safety -> motor command.
    _motorController.stop();
  }

  _motorController.stop();
  _sensors.shutdown();
}
