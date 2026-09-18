# Firmware del Pacesetter

Este directorio contiene el software del ESP32 para controlar las dos tiras LED WS2811 del pacesetter. El proyecto está preparado para compilarse y cargarse con **Arduino IDE**.

## Estructura

```text
firmware/
└── Pacesetter/
    ├── config.h       Configuración de pines, píxeles y velocidad
    └── Pacesetter.ino  Programa principal
```

## Primera versión

La versión inicial implementa:

- ESP32 como microcontrolador.
- Dos tiras WS2811 de 500 píxeles configurables cada una.
- Ventana luminosa de aproximadamente 1 m, equivalente a 20 píxeles.
- Recorrido de ida por la tira derecha.
- Recorrido de vuelta por la tira izquierda.
- Botones para aumentar velocidad, reducir velocidad e iniciar o pausar.
- Antirrebote por software para los botones.
- Brillo inicial limitado para las primeras pruebas eléctricas.

La tira derecha y la izquierda se controlan mediante líneas de datos independientes. El `74AHCT125` debe colocarse entre los GPIO del ESP32 y las entradas `DIN` de las tiras.

## Pines utilizados

| Función | GPIO |
| --- | ---: |
| Datos tira derecha | 18 |
| Datos tira izquierda | 19 |
| Velocidad + | 25 |
| Velocidad - | 26 |
| Inicio/Pausa | 27 |

Los botones se conectan entre su GPIO y `GND`. El programa utiliza `INPUT_PULLUP`.

## Abrir y cargar con Arduino IDE

1. Instalar el soporte de placas ESP32 desde el gestor de tarjetas del Arduino IDE.
2. Instalar la librería `FastLED` desde el gestor de bibliotecas.
3. Abrir el archivo `firmware/Pacesetter/Pacesetter.ino`.
4. Seleccionar la placa `ESP32 Dev Module`.
5. Seleccionar el puerto COM correspondiente.
6. Compilar antes de conectar las tiras completas.
7. Subir el firmware al ESP32.

La primera prueba debe realizarse con un tramo corto de tira LED y una fuente de alimentación limitada. No conectar la batería de 12 V al ESP32.

## Parámetros principales

Los parámetros editables se encuentran en `include/config.h`:

- `NUM_PIXELS`: cantidad de píxeles por tira.
- `WINDOW_PIXELS`: tamaño de la ventana luminosa.
- `DEFAULT_STEP_INTERVAL_MS`: velocidad inicial.
- `MIN_STEP_INTERVAL_MS`: velocidad máxima.
- `MAX_STEP_INTERVAL_MS`: velocidad mínima.
- `LED_BRIGHTNESS`: brillo máximo durante las pruebas.

Este código es la primera base funcional. La calibración de velocidad, el color definitivo y el comportamiento exacto de los extremos se ajustarán mediante pruebas físicas.
