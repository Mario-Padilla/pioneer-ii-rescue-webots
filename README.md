# Pioneer II Rescue

Controlador C++ para la misión de rescate de las prácticas de Robótica, ejecutado en Webots sobre el Pioneer II.

## Misión y restricciones

El robot debe encontrar dos víctimas (cilindros verdes), detenerse a menos de un metro de cada una, señalizar cada rescate con una vuelta completa e imprimir un mensaje por terminal. Después debe regresar al punto de partida, evitando colisiones y completando la misión en un máximo de cuatro minutos.

Hay diez escenarios estáticos. Dentro de cada escenario no cambian las paredes ni los obstáculos; entre ejecuciones cambia la pose inicial (posición y orientación) y el profesor selecciona el escenario. En los mundos actuales, la línea amarilla de inicio está en `z = -8` y la línea amarilla del extremo de las víctimas/meta en `z = +8`. El robot empieza en el lado de salida, pero su posición y orientación exactas se aleatorizan: no tiene por qué estar sobre la línea amarilla. El objetivo de regreso es la línea amarilla de inicio, no la pose exacta donde arrancó el robot. Las líneas amarillas y negras son comunes a todos: sirven como referencia de posición/progreso, no identifican por sí solas el mapa.

## Estrategia prevista

Se preparará fuera de la competición una cuadrícula de ocupación por escenario a partir de la geometría de los mundos `.wbt`. Durante la misión no se recorrerá todo el recinto para construir un mapa desde cero.

1. **Inicializar y estimar la pose.** Leer GPS, brújula, encoders y sensores de distancia. Guardar la pose real medida al inicio como referencia de localización y odometría; el destino de regreso será la línea amarilla de inicio.
2. **Identificar el escenario en movimiento.** Avanzar de forma segura hacia la zona de rescate. Comparar las lecturas de distancia con los mapas candidatos y acumular evidencia desde varias poses. La cámara es una fuente auxiliar; la iluminación no debe ser la señal principal.
3. **Planificar desde la pose actual.** Al alcanzar suficiente confianza, transformar la pose GPS a celda de la cuadrícula elegida. Usar A* para rutas desde la pose actual hacia las víctimas y después hasta una celda alcanzable de la línea amarilla de inicio. Comparar los dos órdenes posibles de visita.
4. **Seguir la ruta con seguridad.** `Navigator` sigue los puntos de la ruta; la evitación reactiva limita o ajusta el movimiento. El seguimiento de paredes se reserva para recuperar el avance cuando un obstáculo lo bloquea. Si las observaciones contradicen la ruta, actualizar la zona local y replanificar.
5. **Rescatar y volver.** Detectar y confirmar visualmente cada víctima, aproximarse a menos de un metro, imprimir el mensaje, girar una vuelta y continuar. Tras ambas, volver a la línea amarilla de inicio; la pose GPS capturada al arrancar es una referencia de localización, no la meta final.

La identificación debe exigir una puntuación suficiente y una ventaja clara sobre el segundo candidato. Si no se identifica el mapa antes del límite de tiempo o distancia acordado, se pasa a un modo seguro de respaldo. Frontier-Based Exploration podría usarse de manera limitada en ese caso; no es la estrategia principal.

## Arquitectura: recorrido de datos

`fool_controller.cpp` es el punto de entrada. Crea un único `RescueController`; este coordina el ciclo de Webots y los módulos. Las clases de planificación y misión son C++ del mismo ejecutable, no controladores Webots independientes.

```text
Webots Robot -> SensorManager -> SensorSnapshot -> Localization -> Pose2D
                                      │                │
                                      └──────┬─────────┘
                                             ▼
MapCatalog ----------------------------> MapMatcher
                                             │
                           Pose + mapa + detecciones de víctima
                                             ▼
VictimDetector ---------------------> MissionManager -> AStarPlanner
                                                           │
                                                           ▼
                                                   Navigator + sensores
                                                           │
                                                           ▼
                                                ObstacleAvoidance
                                                           │
                                                           ▼
                                                    MotorController
```

Los `.h` declaran tipos e interfaces; los `.cpp` contienen su implementación. `RobotConfig.h` centraliza parámetros compartidos. Salvo las dos rutas iniciales completas, las rutas de la tabla son relativas a `controllers/fool_controller/`. El estado indicado describe el código actual, no la funcionalidad futura.

## Para qué sirve cada archivo

| Archivo | Para qué sirve | Estado actual |
|---|---|---|
| `controllers/fool_controller/fool_controller.cpp` | `main()`: instancia `RescueController` y llama a `run()`. Mantiene el nombre de controlador que esperan los mundos. | Implementado como punto de entrada. |
| `controllers/fool_controller/Makefile` | Enumera todos los `.cpp` del controlador y los enlaza con el Makefile de Webots. | Compila el esqueleto. |
| `config/RobotConfig.h` | Nombres de motores, encoders, cámaras y sensores; límites de velocidad, geometría y parámetros de cuadrícula/matching. | Constantes iniciales; umbrales pendientes de calibración. |
| `rescue/RescueController.h` | Declara el coordinador y sus dependencias: robot Webots, sensores, mapa, localización, misión, navegación y motores. | Interfaces conectadas como miembros. |
| `rescue/RescueController.cpp` | Obtiene el timestep, inicializa el control de motores/sensores y ejecuta el bucle de pasos Webots. | El bucle solo detiene el robot; el flujo de misión está pendiente. |
| `sensors/SensorSnapshot.h` | Estructura común para pose/sensores: GPS X/Z, rumbo, encoders, validez y 16 distancias. | Tipo declarado; sus campos no se rellenan todavía. |
| `sensors/SensorManager.h` | Interfaz para habilitar, leer y apagar dispositivos Webots. | Declaraciones preparadas. |
| `sensors/SensorManager.cpp` | Debe adquirir los dispositivos, convertir lecturas a unidades físicas y filtrar ruido. | Constructor presente; `initialize`, `read` y `shutdown` aún son esqueletos. |
| `localization/Localization.h` | Define `Pose2D` (X, Z, rumbo) y la interfaz para pose actual y pose inicial capturada. | Tipos e interfaz declarados. |
| `localization/Localization.cpp` | Debe fusionar GPS, brújula y odometría de encoders, y guardar la pose inicial como referencia de localización. | `update()` todavía devuelve la pose por defecto. |
| `mapping/OccupancyGrid.h` | Define celdas `UNKNOWN/FREE/OCCUPIED`, coordenadas de celda y operaciones de cuadrícula. | Interfaz implementada. |
| `mapping/OccupancyGrid.cpp` | Conversión entre mundo X/Z y celda, acceso a celdas y centros; cuadrícula por defecto de 10 × 20 m a 0,2 m. | Implementado; faltan pruebas y rasterización de obstáculos. |
| `mapping/ScenarioMap.h` | Agrupa el identificador del escenario y su `OccupancyGrid`; declara `VictimLandmark`. | Modelo básico; los landmarks no se asocian aún al mapa. |
| `mapping/ScenarioMap.cpp` | Construye un mapa de escenario y expone su identificador/cuadrícula. | Implementación básica. |
| `mapping/MapCatalog.h` | Interfaz del catálogo de mapas estáticos que usará el reconocedor. | Contenedor declarado. |
| `mapping/MapCatalog.cpp` | Debe cargar los diez mapas generados fuera de línea. | Devuelve actualmente un catálogo vacío. |
| `mapping/MapMatcher.h` | Define el resultado de reconocimiento (mejor/segundo score) y la interfaz de actualización/reset. | Tipos e interfaz declarados. |
| `mapping/MapMatcher.cpp` | Debe comparar rangos observados con ray-casting sobre cada mapa y acumular evidencia a través del movimiento. | Devuelve “no identificado”; algoritmo pendiente. |
| `mapping/data/README.md` | Especifica dónde irán las cuadrículas, su resolución/coordenadas y que las víctimas se guardan como datos semánticos. | Formato previsto; aún no hay mapas generados. |
| `planning/AStarPlanner.h` | Interfaz para encontrar una secuencia de celdas entre inicio y objetivo. | Declaración preparada. |
| `planning/AStarPlanner.cpp` | Debe implementar A*, política para celdas desconocidas y coste de margen/riesgo. | Devuelve una ruta vacía; algoritmo pendiente. |
| `navigation/Navigator.h` | Define `WheelCommand` y la interfaz para transformar objetivos/rutas en velocidades de ruedas. | Tipos e interfaz preparados. |
| `navigation/Navigator.cpp` | Debe seguir waypoints y corregir rumbo/velocidad. | Constructor vacío; navegación pendiente. |
| `navigation/ObstacleAvoidance.h` | Interfaz de seguridad local basada en distancias. | Declaración preparada. |
| `navigation/ObstacleAvoidance.cpp` | Debe limitar o modificar el comando de navegación ante obstáculos y desbloquear progreso. | Constructor vacío; algoritmo pendiente. |
| `perception/VictimDetector.h` | Interfaz de detección de cilindros verdes; deberá entregar confianza, dirección y estimación de distancia. | Interfaz pendiente de definir completamente. |
| `perception/VictimDetector.cpp` | Debe procesar las cámaras frontal/esférica y confirmar detecciones a través de varios fotogramas. | Constructor vacío; visión pendiente. |
| `mission/MissionManager.h` | Enumera estados: inicialización, identificación, planificación, búsqueda, acercamiento, señalización, regreso, respaldo y fin. | Estados declarados. |
| `mission/MissionManager.cpp` | Debe aplicar transiciones según pose, mapa, detecciones y víctimas rescatadas. | Solo inicializa y devuelve `INITIALIZING`. |
| `control/MotorController.h` | Interfaz de inicialización, velocidades de ruedas y parada segura. | Interfaz implementada. |
| `control/MotorController.cpp` | Adquiere los motores, configura velocidad continua, limita velocidades y permite detenerlos. | Parte funcional; es el módulo con control de hardware actualmente activo. |
| `docs/README.md` | Índice del enunciado e informe del equipo. | Enunciado PDF guardado en `docs/`. |
| `tools/README.md` | Describe las herramientas offline previstas, especialmente el generador de mapas desde `.wbt`. | Generador aún pendiente. |
| `tests/README.md` | Sugiere pruebas unitarias para cuadrícula, A* y matching. | Pruebas aún pendientes. |

## Checklist del proceso de desarrollo

Este es el orden recomendado para construir, probar e integrar el software. No describe una secuencia de acciones del robot durante la misión.

- [x] **Preparar el proyecto:** separar módulos, configurar el Makefile y compilar el esqueleto en Webots.
- [ ] **Validar la interfaz con Webots:** implementar lecturas y ciclo de vida de dispositivos; crear una prueba/registro de sensores.
- [ ] **Caracterizar y modelar sensores:** verificar convenciones X/Z, rumbo, sectores de los 16 sensores y conversión de distancias; documentar ruido y unidades.
- [ ] **Implementar localización:** fusionar GPS, brújula y encoders; capturar la pose real del robot al iniciar (no las coordenadas de la línea amarilla) y probarla en movimientos conocidos.
- [ ] **Crear los mapas de referencia:** escribir la herramienta offline que rasteriza paredes/obstáculos de los diez `.wbt`; validar visualmente las cuadrículas y cargarlas en `MapCatalog`.
- [ ] **Implementar y probar el reconocedor:** comparar lecturas con ray-casting por mapa, acumular evidencia, definir confianza/margen y probar escenarios similares desde varias poses.
- [ ] **Implementar y probar A*:** añadir margen del robot, política de celdas desconocidas, casos sin ruta y cálculo del coste de ambos órdenes de visita.
- [ ] **Implementar navegación local:** seguir waypoints, limitar velocidades, evitar colisiones y recuperar el avance; probar por separado en recorridos controlados.
- [ ] **Implementar percepción de víctimas:** probar procesamiento de cámara con iluminación/niebla distintas; producir detecciones estables con dirección y estimación de rango.
- [ ] **Implementar la máquina de misión:** conectar reconocimiento, planificación, detección, rescate, señalización, regreso y estado seguro.
- [ ] **Integrar y ajustar:** ejecutar la misión completa, corregir fallos y ajustar umbrales/velocidades sin sobreajustar a un solo escenario.
- [ ] **Validar y documentar resultados:** ejecutar pruebas repetidas en los diez escenarios y registrar tiempo, víctimas, regreso y colisiones para el informe.

## Checklist por objetivo de desarrollo

### Objetivo 1 — Sensores, hardware y localización

- [ ] Implementar `SensorManager::initialize`, `read` y `shutdown` para GPS, brújula, encoders y sensores de distancia.
- [ ] Completar `SensorSnapshot` con lecturas válidas en unidades físicas y documentar el orden/sector de cada sensor.
- [ ] Implementar `Localization::update` y almacenar la pose GPS real al iniciar como referencia de localización.
- [ ] Añadir pruebas/registro de calibración: tras movimientos conocidos, posición y rumbo estimados siguen el movimiento esperado.

### Objetivo 2 — Generación y carga de mapas

- [ ] Implementar en `tools/` un conversor de geometría de los mundos a cuadrículas X/Z de 0,2 m.
- [ ] Rasterizar bounding objects con rotaciones; excluir suelo y marcas del suelo, y representar víctimas como objetivos semánticos.
- [ ] Inflar obstáculos por la huella del robot y el margen de seguridad.
- [ ] Implementar carga de los diez mapas en `MapCatalog`; comprobar cada cuadrícula contra el escenario Webots correspondiente.

### Objetivo 3 — Identificación del escenario

- [ ] Añadir al mapa el modelo necesario para predecir lecturas de distancia desde una pose.
- [ ] Implementar en `MapMatcher` el score acumulativo y el criterio de mejor puntuación + margen sobre el segundo candidato.
- [ ] Conectar identificación en movimiento con un límite de tiempo/distancia y modo de respaldo.
- [ ] Validar con varias poses y orientaciones, prestando atención a los pares 02/03 y 06/07; registrar falsos positivos y tiempo hasta identificar.

### Objetivo 4 — Planificador global A*

- [ ] Implementar `AStarPlanner::findPath` y definir conectividad, coste y política para celdas desconocidas.
- [ ] Añadir pruebas para ruta libre, obstáculo, margen/inflación y objetivo inalcanzable.
- [ ] Calcular rutas desde la pose actual hacia las dos víctimas y de vuelta a una celda alcanzable de la línea amarilla de inicio; comparar ambos órdenes de visita.
- [ ] Verificar que cada ruta es continua, empieza/termina en las celdas pedidas y no atraviesa obstáculos inflados.

### Objetivo 5 — Seguimiento de ruta y seguridad

- [ ] Implementar `Navigator` para convertir waypoints/pose en comandos de ruedas.
- [ ] Implementar `ObstacleAvoidance` como límite de seguridad/reactivo, no como planificador global competidor.
- [ ] Añadir recuperación por seguimiento de pared solo ante bloqueo y lógica de actualización local/replanificación.
- [ ] Probar primero segmentos y obstáculos aislados; después verificar que no hay contactos en recorridos integrados.

### Objetivo 6 — Detección y manejo de víctimas

- [ ] Definir salida de `VictimDetector` (confianza, ángulo respecto al robot y rango/estimación de distancia).
- [ ] Implementar segmentación/detección de cilindros verdes con confirmación temporal y tolerancia a cambios de iluminación y niebla.
- [ ] Verificar aproximación a menos de 1 m, mensaje de terminal y vuelta completa medible.
- [ ] Comprobar que cada víctima se contabiliza una sola vez y que su detección no se confunde con obstáculos.

### Objetivo 7 — Integración, robustez y evaluación

- [ ] Implementar transiciones de `MissionManager` y conectar módulos en `RescueController`.
- [ ] Comprobar recorrido de misión completo: salida aleatoria, dos rescates, regreso a la línea amarilla de inicio y parada segura.
- [ ] Ejecutar repeticiones en los diez mundos con distintas poses iniciales y límite de cuatro minutos.
- [ ] Guardar métricas por ejecución y usar resultados para ajustar parámetros y preparar informe/presentación.

## Estructura del proyecto

```text
Proyecto_Final/
├── controllers/fool_controller/  # un ejecutable Webots y sus módulos C++
│   ├── config/ control/ localization/ mapping/ mission/
│   ├── navigation/ perception/ planning/ rescue/ sensors/
│   └── mapping/data/             # mapas que carga el controlador
├── worlds/                       # diez escenarios Webots
├── protos/Pioneer2_Robot.proto   # modelo del Pioneer II
├── libraries/                    # bibliotecas del proyecto Webots
├── plugins/                      # plugins del proyecto Webots
├── tools/                        # utilidades offline
├── tests/                        # pruebas independientes del simulador
├── docs/                         # enunciado e informe
└── README.md
```

El enunciado oficial está en [`docs/enunciado_trabajo_final.pdf`](docs/enunciado_trabajo_final.pdf). Las carpetas `tools/`, `tests/` y `docs/` tienen un README propio.

## Configuración, compilación y ejecución

La cuadrícula está configurada para el recinto de 10 × 20 m, origen X/Z `(-5, -10)` y celdas de 0,2 m (50 × 100). Al rasterizar obstáculos se añadirá margen por el radio del robot y la incertidumbre. Los mundos seleccionan el controlador `fool_controller`; no renombrar su carpeta sin actualizar los mundos.

Con Webots instalado y `WEBOTS_HOME` configurado:

```sh
cd controllers/fool_controller
make
```

También se puede abrir cualquiera de los mundos y usar **Build** sobre el controlador `fool_controller`.

## Validación y resultados

Probar en los diez mundos y con varias posiciones/orientaciones iniciales. Registrar por ejecución: escenario, pose real inicial y su relación con la línea amarilla de inicio, mapa identificado y distancia recorrida antes de identificarlo, víctimas rescatadas, tiempo total, distancia de regreso y colisiones. El criterio de regreso es alcanzar la línea amarilla de inicio, no volver a la pose GPS exacta de partida. Prestar atención a pares geométricamente similares, especialmente 02/03 y 06/07. Los datos por escenario se usarán para ajustar el controlador y preparar el informe.
