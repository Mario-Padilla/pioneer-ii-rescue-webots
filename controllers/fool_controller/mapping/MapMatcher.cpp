#include "MapMatcher.h"

MapMatcher::MapMatcher() {}

MapMatchResult MapMatcher::update(
    const Pose2D &pose, const SensorSnapshot &sensors,
    const std::vector<ScenarioMap> &candidates) {
  // TODO: compare measured ranges with candidate-map ray casts. Lines provide
  // common pose references; obstacle geometry distinguishes the scenarios.
  (void)pose;
  (void)sensors;
  (void)candidates;
  return MapMatchResult();
}

void MapMatcher::reset() {
  _logLikelihoods.clear();
}
