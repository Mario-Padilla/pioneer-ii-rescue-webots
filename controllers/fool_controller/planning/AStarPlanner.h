#ifndef RESCUE_A_STAR_PLANNER_H_
#define RESCUE_A_STAR_PLANNER_H_

#include <vector>

#include "../mapping/OccupancyGrid.h"

class AStarPlanner {
public:
  AStarPlanner();

  std::vector<GridCell> findPath(const OccupancyGrid &map,
                                 const GridCell &start,
                                 const GridCell &goal) const;

  // TODO: implement A* with obstacle inflation and explicit unknown-cell policy.
};

#endif  // RESCUE_A_STAR_PLANNER_H_
