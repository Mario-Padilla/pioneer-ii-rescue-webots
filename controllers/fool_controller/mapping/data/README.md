# Mapas de ocupación estáticos

Aquí se guardarán las diez cuadrículas rasterizadas a partir de `worlds/final_scenario01.wbt` ... `final_scenario10.wbt`.

Formato previsto: una celda por cada 0,2 m, coordenadas de mundo X/Z, con valores libre/ocupado. Las líneas del suelo no se rasterizan como obstáculos; se usan como referencias comunes. Las víctimas se guardan como objetivos semánticos separados de la geometría de colisión.

El generador y el cargador todavía están pendientes. Las dimensiones y el origen de la cuadrícula se definen en `../../config/RobotConfig.h`.
