#include "RescueController.h"

#include <chrono>
#include <cmath>
#include <ctime>
#include <cerrno>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

namespace {

bool makeDirectory(const std::string &path) {
#ifdef _WIN32
  return _mkdir(path.c_str()) == 0 || errno == EEXIST;
#else
  return mkdir(path.c_str(), 0775) == 0 || errno == EEXIST;
#endif
}

bool ensureDiagnosticsDirectory(const std::string &projectPath,
                                const std::string &diagnosticsDirectory) {
  if (projectPath.empty())
    return false;
  return makeDirectory(projectPath + "/results") &&
         makeDirectory(diagnosticsDirectory);
}

std::string makeRunId() {
  const std::time_t now = std::time(0);
  const std::tm *localTime = std::localtime(&now);
  if (localTime == 0)
    return "sensor_run";

  const long milliseconds = static_cast<long>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count() % 1000);
  std::ostringstream runId;
  runId << std::put_time(localTime, "%Y%m%d_%H%M%S") << '_'
        << std::setfill('0') << std::setw(3) << milliseconds;
  return runId.str();
}

bool saveCameraImage(const webots::Camera *camera, const std::string &path) {
  if (camera == 0)
    return false;

  const unsigned char *image = camera->getImage();
  if (image == 0)
    return false;

  const int width = camera->getWidth();
  const int height = camera->getHeight();
  std::ofstream output(path.c_str(), std::ios::out | std::ios::binary);
  if (!output)
    return false;

  output << "P6\n" << width << ' ' << height << "\n255\n";
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const unsigned char rgb[3] = {
          webots::Camera::imageGetRed(image, width, x, y),
          webots::Camera::imageGetGreen(image, width, x, y),
          webots::Camera::imageGetBlue(image, width, x, y)};
      output.write(reinterpret_cast<const char *>(rgb), sizeof(rgb));
    }
  }

  return output.good();
}

void printSnapshot(const SensorSnapshot &snapshot,
                   const ObstacleAssessment &obstacleAssessment,
                   double simulationTimeS) {
  std::cout << std::fixed << std::setprecision(2)
            << "SENSORS t=" << simulationTimeS << "s";

  if (snapshot.gpsValid)
    std::cout << " GPS=(" << snapshot.gpsXM << ", " << snapshot.gpsZM << ")m";
  else
    std::cout << " GPS=INVALID";

  if (snapshot.compassValid) {
    const double headingDegrees = snapshot.compassHeadingRad * 180.0 / M_PI;
    std::cout << " compass=" << headingDegrees << "deg";
  } else {
    std::cout << " compass=INVALID";
  }

  if (snapshot.encodersValid) {
    std::cout << " encoders=(" << snapshot.leftEncoderRad << ", "
              << snapshot.rightEncoderRad << ")rad";
  } else {
    std::cout << " encoders=INVALID";
  }

  if (snapshot.distanceSensorsValid) {
    std::cout << " RANGE(raw)=";
    for (int i = 0; i < robot_config::kDistanceSensorCount; ++i) {
      if (i != 0)
        std::cout << ',';
      std::cout << "ds" << i << ':' << snapshot.distanceSensorValues[i];
    }
  } else {
    std::cout << " RANGE=INVALID";
  }

  if (snapshot.distanceSensorsValid) {
    std::cout << " RANGE(m)=";
    for (int i = 0; i < robot_config::kDistanceSensorCount; ++i) {
      if (i != 0)
        std::cout << ',';
      std::cout << "ds" << i << ':';
      if (!snapshot.distanceDetected[i]) {
        std::cout << ">=" << robot_config::distanceSensorMaximumRangeM(i);
      } else if (snapshot.distanceNearSaturated[i]) {
        std::cout << "<= " << robot_config::distanceSensorNearSaturationM(i);
      } else {
        std::cout << snapshot.distanceMeters[i];
      }
    }
  }

  if (snapshot.distanceSectors.valid) {
    const DistanceSensorSectors &sectors = snapshot.distanceSectors;
    std::cout << " SECTORS(raw)="
              << "front:" << sectors.front
              << "(L:" << sectors.frontLeft
              << ",R:" << sectors.frontRight << ')'
              << ",diag(L:" << sectors.diagonalLeft
              << ",R:" << sectors.diagonalRight << ')'
              << ",walls(L:" << sectors.leftWall
              << ",R:" << sectors.rightWall << ')'
              << ",rear(L:" << sectors.rearLeft
              << ",R:" << sectors.rearRight << ')';
  }

  if (snapshot.distanceSensorsValid) {
    const DistanceMetersSectors &sectors = snapshot.distanceMetersSectors;
    const auto printSectorDistance = [](const DistanceSectorReading &reading) {
      if (!std::isfinite(reading.distanceM)) {
        std::cout << "clear";
      } else if (reading.nearSaturated) {
        std::cout << "<= " << reading.nearSaturationLimitM;
      } else {
        std::cout << reading.distanceM;
      }
    };
    std::cout << " SECTORS(m)="
              << "front:";
    printSectorDistance(sectors.front);
    std::cout << "(L:";
    printSectorDistance(sectors.frontLeft);
    std::cout << ",R:";
    printSectorDistance(sectors.frontRight);
    std::cout << "),diag(L:";
    printSectorDistance(sectors.diagonalLeft);
    std::cout << ",R:";
    printSectorDistance(sectors.diagonalRight);
    std::cout << "),walls(L:";
    printSectorDistance(sectors.leftWall);
    std::cout << ",R:";
    printSectorDistance(sectors.rightWall);
    std::cout << "),rear(L:";
    printSectorDistance(sectors.rearLeft);
    std::cout << ",R:";
    printSectorDistance(sectors.rearRight);
    std::cout << ')';
  }

  if (obstacleAssessment.valid) {
    std::cout << " P3(raw):warning=" << obstacleAssessment.frontWarning
              << ",frontObstacle=" << obstacleAssessment.frontObstacle
              << ",frontClear=" << obstacleAssessment.frontClear
              << ",sideDanger(L:" << obstacleAssessment.leftDanger
              << ",R:" << obstacleAssessment.rightDanger << ')'
              << ",wall(L:" << obstacleAssessment.leftWallDetected
              << ",R:" << obstacleAssessment.rightWallDetected << ')';
    if (obstacleAssessment.frontObstacle) {
      std::cout << ",risk(L:" << obstacleAssessment.leftRiskRaw
                << ",R:" << obstacleAssessment.rightRiskRaw << ')'
                << ",turn="
                << (obstacleAssessment.suggestedTurn == AvoidanceTurn::LEFT
                        ? "LEFT"
                        : "RIGHT");
    }
  }

  std::cout << std::endl;
}

}  // namespace

RescueController::RescueController()
    : _sensors(&_robot),
      _localization(),
      _mapCatalog(),
      _mapMatcher(),
      _planner(),
      _navigator(),
      _missionManager(),
      _motorController(&_robot) {}

void RescueController::run() {
  const int timeStep = static_cast<int>(_robot.getBasicTimeStep());
  const std::string runId = makeRunId();

  // Safe scaffold: the planned flow is to identify the static map while
  // moving safely, plan from the current pose, and follow the route.
  _motorController.initialize();
  if (!_sensors.initialize(timeStep)) {
    _motorController.stop();
    return;
  }

  const std::string diagnosticsDirectory =
      _robot.getProjectPath() + "/results/sensor_diagnostics";
  const bool diagnosticsDirectoryReady =
      ensureDiagnosticsDirectory(_robot.getProjectPath(), diagnosticsDirectory);
  std::cout << "Sensor diagnostic run " << runId << ". Camera snapshots will "
            << (diagnosticsDirectoryReady ? "be written under "
                                          : "not be saved; could not create ")
            << diagnosticsDirectory << "." << std::endl;

  const int reportEverySteps = timeStep > 0 ? (1000 + timeStep - 1) / timeStep : 1;
  int stepCount = 0;
  bool cameraSnapshotsWritten = false;
  while (_robot.step(timeStep) != -1) {
    const SensorSnapshot snapshot = _sensors.read();
    const ObstacleAssessment obstacleAssessment =
        _obstacleAvoidance.evaluate(snapshot.distanceSectors);
    if (stepCount % reportEverySteps == 0) {
      printSnapshot(snapshot, obstacleAssessment, _robot.getTime());
    }

    if (!cameraSnapshotsWritten) {
      webots::Camera *forwardCamera = _sensors.forwardCamera();
      webots::Camera *sphericalCamera = _sensors.sphericalCamera();
      const std::string forwardPath =
          diagnosticsDirectory + "/camera_f_" + runId + ".ppm";
      const std::string sphericalPath =
          diagnosticsDirectory + "/camera_s_" + runId + ".ppm";
      const bool forwardSaved = diagnosticsDirectoryReady &&
          saveCameraImage(forwardCamera, forwardPath);
      const bool sphericalSaved = diagnosticsDirectoryReady &&
          saveCameraImage(sphericalCamera, sphericalPath);
      std::cout << "Camera camera_f "
                << (forwardSaved ? "saved: " : "could not save: ")
                << forwardPath;
      if (forwardCamera != 0)
        std::cout << " (" << forwardCamera->getWidth() << 'x'
                  << forwardCamera->getHeight() << ')';
      std::cout << std::endl;
      std::cout << "Camera camera_s "
                << (sphericalSaved ? "saved: " : "could not save: ")
                << sphericalPath;
      if (sphericalCamera != 0)
        std::cout << " (" << sphericalCamera->getWidth() << 'x'
                  << sphericalCamera->getHeight() << ')';
      std::cout << std::endl;
      cameraSnapshotsWritten = true;
    }

    ++stepCount;
    // TODO: localization -> progressive map matching -> mission.
    // Once matched: A* route -> reactive safety -> motor command.
    _motorController.stop();
  }

  _motorController.stop();
  _sensors.shutdown();
}
