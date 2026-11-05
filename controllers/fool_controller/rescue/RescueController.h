#ifndef RESCUE_CONTROLLER_H_
#define RESCUE_CONTROLLER_H_

#include <webots/Robot.hpp>

#include "../control/MotorController.h"
#include "../localization/Localization.h"
#include "../mapping/MapCatalog.h"
#include "../mapping/MapMatcher.h"
#include "../mission/MissionManager.h"
#include "../navigation/Navigator.h"
#include "../perception/VictimDetector.h"
#include "../planning/AStarPlanner.h"
#include "../sensors/SensorManager.h"

class RescueController {
public:
  RescueController();
  void run();

private:
  webots::Robot _robot;
  SensorManager _sensors;
  VictimDetector _victimDetector;
  Localization _localization;
  MapCatalog _mapCatalog;
  MapMatcher _mapMatcher;
  AStarPlanner _planner;
  Navigator _navigator;
  MissionManager _missionManager;
  MotorController _motorController;
};

#endif  // RESCUE_CONTROLLER_H_
