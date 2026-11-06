# Pruebas

Las pruebas unitarias o de integración de módulos independientes del simulador van aquí.

Primeros candidatos pendientes: conversión mundo/cuadrícula de `OccupancyGrid`, planificación A* y cálculo de puntuación de `MapMatcher`.

La conversión inversa de los sensores de distancia y la agregación sectorial tienen una prueba independiente de Webots. Comprueba los perfiles láser y de alcance largo/corto, interpolación, ausencia de retorno, saturación cercana y mínimo por sector. Desde la raíz del proyecto:

```sh
g++ -std=c++11 tests/sensor_calibration_test.cpp \
  controllers/fool_controller/sensors/DistanceSensorCalibration.cpp \
  -o /tmp/sensor_calibration_test
/tmp/sensor_calibration_test
```
