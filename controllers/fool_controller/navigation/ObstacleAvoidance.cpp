#include "ObstacleAvoidance.h"

#include <algorithm>

#include "../config/RobotConfig.h"

ObstacleAvoidance::ObstacleAvoidance() {}

ObstacleAssessment ObstacleAvoidance::evaluate(
    const DistanceSensorSectors &sectors) const {
  ObstacleAssessment assessment;
  if (!sectors.valid)
    return assessment;

  assessment.valid = true;
  assessment.frontWarning =
      sectors.front > robot_config::kFrontWarningThresholdRaw;
  assessment.frontObstacle =
      sectors.front > robot_config::kFrontObstacleThresholdRaw;
  assessment.frontClear =
      sectors.front < robot_config::kFrontClearThresholdRaw;
  assessment.leftDanger =
      sectors.leftWall > robot_config::kSideDangerThresholdRaw;
  assessment.rightDanger =
      sectors.rightWall > robot_config::kSideDangerThresholdRaw;
  assessment.leftWallDetected =
      sectors.leftWall > robot_config::kWallDetectThresholdRaw;
  assessment.rightWallDetected =
      sectors.rightWall > robot_config::kWallDetectThresholdRaw;

  if (assessment.frontObstacle) {
    assessment.leftRiskRaw =
        0.50 * std::max(sectors.frontLeft, sectors.diagonalLeft) +
        0.35 * sectors.leftWall + 0.15 * sectors.rearLeft;
    assessment.rightRiskRaw =
        0.50 * std::max(sectors.frontRight, sectors.diagonalRight) +
        0.35 * sectors.rightWall + 0.15 * sectors.rearRight;
    assessment.suggestedTurn = assessment.leftRiskRaw < assessment.rightRiskRaw
                                   ? AvoidanceTurn::LEFT
                                   : AvoidanceTurn::RIGHT;
  }

  return assessment;
}
