#ifndef RESCUE_MAP_MATCHER_H_
#define RESCUE_MAP_MATCHER_H_

#include <vector>

#include "ScenarioMap.h"
#include "../localization/Localization.h"
#include "../sensors/SensorSnapshot.h"

struct MapMatchResult {
  MapMatchResult()
      : scenarioId(-1), bestScore(0.0), secondBestScore(0.0), identified(false) {}

  int scenarioId;
  double bestScore;
  double secondBestScore;
  bool identified;
};

class MapMatcher {
public:
  MapMatcher();

  MapMatchResult update(const Pose2D &pose, const SensorSnapshot &sensors,
                        const std::vector<ScenarioMap> &candidates);
  void reset();

  // TODO: accumulate sensor-model likelihoods for candidate maps over time.

private:
  std::vector<double> _logLikelihoods;
};

#endif  // RESCUE_MAP_MATCHER_H_
