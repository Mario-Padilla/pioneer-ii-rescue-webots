#ifndef RESCUE_OBSTACLE_AVOIDANCE_H_
#define RESCUE_OBSTACLE_AVOIDANCE_H_

#include "../sensors/SensorSnapshot.h"

enum class AvoidanceTurn { NONE, LEFT, RIGHT };

struct ObstacleAssessment {
  ObstacleAssessment()
      : valid(false),
        frontWarning(false),
        frontObstacle(false),
        frontClear(false),
        leftDanger(false),
        rightDanger(false),
        leftWallDetected(false),
        rightWallDetected(false),
        leftRiskRaw(0.0),
        rightRiskRaw(0.0),
        suggestedTurn(AvoidanceTurn::NONE) {}

  bool valid;
  bool frontWarning;
  bool frontObstacle;
  bool frontClear;
  bool leftDanger;
  bool rightDanger;
  bool leftWallDetected;
  bool rightWallDetected;
  double leftRiskRaw;
  double rightRiskRaw;
  AvoidanceTurn suggestedTurn;
};

class ObstacleAvoidance {
public:
  ObstacleAvoidance();

  // Classifies the practice-03 raw sensor sectors without commanding motion.
  ObstacleAssessment evaluate(const DistanceSensorSectors &sectors) const;
};

#endif  // RESCUE_OBSTACLE_AVOIDANCE_H_
