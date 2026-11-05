#ifndef RESCUE_MISSION_MANAGER_H_
#define RESCUE_MISSION_MANAGER_H_

enum class MissionState {
  INITIALIZING,
  IDENTIFYING_MAP,
  PLANNING_ROUTE,
  SEARCHING_FOR_VICTIMS,
  APPROACHING_VICTIM,
  SIGNALING_RESCUE,
  RETURNING_TO_START,
  SAFE_FALLBACK,
  COMPLETED
};

class MissionManager {
public:
  MissionManager();

  MissionState state() const;

  // TODO: update state from pose, victim detections, and rescue progress.

private:
  MissionState _state;
};

#endif  // RESCUE_MISSION_MANAGER_H_
