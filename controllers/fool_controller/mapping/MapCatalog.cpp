#include "MapCatalog.h"

MapCatalog::MapCatalog() {}

const std::vector<ScenarioMap> &MapCatalog::maps() const {
  return _maps;
}
