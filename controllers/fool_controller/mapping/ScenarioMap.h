#ifndef RESCUE_SCENARIO_MAP_H_
#define RESCUE_SCENARIO_MAP_H_

#include "OccupancyGrid.h"

struct VictimLandmark {
  VictimLandmark() : xM(0.0), zM(0.0) {}
  VictimLandmark(double x, double z) : xM(x), zM(z) {}

  double xM;
  double zM;
};

class ScenarioMap {
public:
  ScenarioMap();
  explicit ScenarioMap(int scenarioId);

  int scenarioId() const;
  const OccupancyGrid &grid() const;
  OccupancyGrid &grid();

private:
  int _scenarioId;
  OccupancyGrid _grid;
};

#endif  // RESCUE_SCENARIO_MAP_H_
