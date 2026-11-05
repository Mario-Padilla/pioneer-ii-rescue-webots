#ifndef RESCUE_MAP_CATALOG_H_
#define RESCUE_MAP_CATALOG_H_

#include <vector>

#include "ScenarioMap.h"

class MapCatalog {
public:
  MapCatalog();

  const std::vector<ScenarioMap> &maps() const;

  // TODO: load occupancy grids generated offline from the ten Webots worlds.

private:
  std::vector<ScenarioMap> _maps;
};

#endif  // RESCUE_MAP_CATALOG_H_
