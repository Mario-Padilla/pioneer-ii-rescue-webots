#ifndef RESCUE_OCCUPANCY_GRID_H_
#define RESCUE_OCCUPANCY_GRID_H_

#include <cstddef>
#include <vector>

enum class CellState {
  UNKNOWN,
  FREE,
  OCCUPIED
};

struct GridCell {
  GridCell() : column(0), row(0) {}
  GridCell(int columnValue, int rowValue)
      : column(columnValue), row(rowValue) {}

  int column;
  int row;
};

class OccupancyGrid {
public:
  OccupancyGrid();
  OccupancyGrid(std::size_t columns, std::size_t rows, double cellSizeM,
                double originXM, double originZM,
                CellState initialState = CellState::UNKNOWN);

  bool contains(const GridCell &cell) const;
  CellState at(const GridCell &cell) const;
  void set(const GridCell &cell, CellState state);
  bool worldToCell(double xM, double zM, GridCell *cell) const;
  void cellCenter(const GridCell &cell, double *xM, double *zM) const;

  std::size_t columns() const;
  std::size_t rows() const;
  double cellSizeM() const;
  double originXM() const;
  double originZM() const;

private:
  std::size_t indexOf(const GridCell &cell) const;

  std::size_t _columns;
  std::size_t _rows;
  double _cellSizeM;
  double _originXM;
  double _originZM;
  std::vector<CellState> _cells;
};

#endif  // RESCUE_OCCUPANCY_GRID_H_
