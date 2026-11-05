#include "ScenarioMap.h"

ScenarioMap::ScenarioMap() : _scenarioId(-1), _grid() {}

ScenarioMap::ScenarioMap(int scenarioId)
    : _scenarioId(scenarioId), _grid() {}

int ScenarioMap::scenarioId() const { return _scenarioId; }

const OccupancyGrid &ScenarioMap::grid() const { return _grid; }

OccupancyGrid &ScenarioMap::grid() { return _grid; }
