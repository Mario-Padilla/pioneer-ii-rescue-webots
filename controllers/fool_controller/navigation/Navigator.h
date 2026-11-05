#ifndef RESCUE_NAVIGATOR_H_
#define RESCUE_NAVIGATOR_H_

struct WheelCommand {
  double leftRadPerSec;
  double rightRadPerSec;
};

class Navigator {
public:
  Navigator();

  // TODO: compute a safe wheel command toward a mission-provided target.
};

#endif  // RESCUE_NAVIGATOR_H_
