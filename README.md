# Dev_Trabajo_conjunto

Esta rama está destinada al **desarrollo colaborativo del software** del proyecto Pacesetter para Piscina.

Aquí se incorporarán, probarán y documentarán todos los archivos relacionados con la programación del sistema: firmware del ESP32, control de las tiras LED WS2811, lectura de botones, regulación de velocidad, rutinas de barrido y pruebas de funcionamiento.

## Propósito de la rama

La rama `Dev_Trabajo_conjunto` funcionará como espacio de trabajo para desarrollar el código de manera ordenada y permitir la colaboración entre los integrantes del proyecto.

En esta rama se podrán:

- Crear y modificar el firmware del ESP32.
- Implementar la secuencia de barrido de las tiras LED.
- Configurar el control de velocidad mediante botones.
- Realizar pruebas antes de incorporar cambios definitivos al proyecto.
- Documentar decisiones técnicas, conexiones y procedimientos de software.
- Revisar y mejorar el código de forma conjunta.

## Contenido esperado

La organización prevista para el software es:

```text
firmware/       Código del ESP32
tests/          Pruebas y validaciones
docs/software/  Documentación específica del software
Instrucciones/  Documentación técnica del sistema
```

La estructura podrá ampliarse a medida que avance el desarrollo.

## Hardware objetivo

El software de esta rama estará preparado inicialmente para trabajar con:

- NodeMCU ESP32 de 38 pines.
- Dos tiras LED WS2811 RGB de 12 V.
- Dos líneas de datos independientes para las tiras.
- Tres botones: aumentar velocidad, reducir velocidad e iniciar o pausar.

## Flujo de trabajo

Los cambios de software deberán probarse primero en esta rama. Una vez verificados, podrán integrarse a `main` cuando se considere que están listos para formar parte de una versión estable.

La rama `main` conservará la información general y descriptiva del producto. Esta rama concentrará el desarrollo colaborativo, el código y la documentación relacionada con el funcionamiento interno del sistema.

## Estado actual

La rama se encuentra preparada para comenzar el desarrollo del firmware y las primeras pruebas con el ESP32 y las tiras WS2811.
