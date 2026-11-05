#include "OccupancyGrid.h"

#include <cmath>

#include "../config/RobotConfig.h"

OccupancyGrid::OccupancyGrid()
    : OccupancyGrid(
          static_cast<std::size_t>(robot_config::kMapWidthM /
                                   robot_config::kMapCellSizeM),
          static_cast<std::size_t>(robot_config::kMapLengthM /
                                   robot_config::kMapCellSizeM),
          robot_config::kMapCellSizeM, robot_config::kMapOriginXM,
          robot_config::kMapOriginZM, CellState::UNKNOWN) {}

OccupancyGrid::OccupancyGrid(std::size_t columnsValue, std::size_t rowsValue,
                             double cellSize, double originX, double originZ,
                             CellState initialState)
    : _columns(columnsValue),
      _rows(rowsValue),
      _cellSizeM(cellSize),
      _originXM(originX),
      _originZM(originZ),
      _cells(columnsValue * rowsValue, initialState) {}

bool OccupancyGrid::contains(const GridCell &cell) const {
  return cell.column >= 0 && cell.row >= 0 &&
         static_cast<std::size_t>(cell.column) < _columns &&
         static_cast<std::size_t>(cell.row) < _rows;
}

CellState OccupancyGrid::at(const GridCell &cell) const {
  if (!contains(cell))
    return CellState::OCCUPIED;
  return _cells[indexOf(cell)];
}

void OccupancyGrid::set(const GridCell &cell, CellState state) {
  if (contains(cell))
    _cells[indexOf(cell)] = state;
}

bool OccupancyGrid::worldToCell(double xM, double zM, GridCell *cell) const {
  if (cell == 0 || _cellSizeM <= 0.0)
    return false;

  const int column = static_cast<int>(std::floor((xM - _originXM) / _cellSizeM));
  const int row = static_cast<int>(std::floor((zM - _originZM) / _cellSizeM));
  const GridCell candidate(column, row);
  if (!contains(candidate))
    return false;

  *cell = candidate;
  return true;
}

void OccupancyGrid::cellCenter(const GridCell &cell, double *xM,
                               double *zM) const {
  if (!contains(cell) || xM == 0 || zM == 0)
    return;

  *xM = _originXM + (cell.column + 0.5) * _cellSizeM;
  *zM = _originZM + (cell.row + 0.5) * _cellSizeM;
}

std::size_t OccupancyGrid::columns() const { return _columns; }
std::size_t OccupancyGrid::rows() const { return _rows; }
double OccupancyGrid::cellSizeM() const { return _cellSizeM; }
double OccupancyGrid::originXM() const { return _originXM; }
double OccupancyGrid::originZM() const { return _originZM; }

std::size_t OccupancyGrid::indexOf(const GridCell &cell) const {
  return static_cast<std::size_t>(cell.row) * _columns +
         static_cast<std::size_t>(cell.column);
}
