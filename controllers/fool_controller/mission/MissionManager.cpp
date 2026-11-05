#include "MissionManager.h"

MissionManager::MissionManager() : _state(MissionState::INITIALIZING) {}

MissionState MissionManager::state() const {
  return _state;
}
