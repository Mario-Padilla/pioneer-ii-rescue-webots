#include "AStarPlanner.h"

AStarPlanner::AStarPlanner() {}

std::vector<GridCell> AStarPlanner::findPath(const OccupancyGrid &map,
                                             const GridCell &start,
                                             const GridCell &goal) const {
  // TODO: return a path minimizing distance plus clearance/risk costs.
  (void)map;
  (void)start;
  (void)goal;
  return std::vector<GridCell>();
}
