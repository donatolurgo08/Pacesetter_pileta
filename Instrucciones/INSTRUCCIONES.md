# Instrucciones del Pacesetter para Piscina

## 1. Objetivo

El sistema funcionará como una liebre de natación o pacesetter. Dos tiras LED WS2811 instaladas sobre las guías flotantes de la piscina mostrarán un barrido luminoso que servirá como referencia visual para regular el ritmo de los nadadores.

La velocidad del barrido se modificará mediante botones.

## 2. Configuración prevista

- Dos tiras LED WS2811 RGB de 12 V y 60 LED por metro.
- Cada tira tendrá una longitud operativa aproximada de 25 m.
- Las tiras compradas pueden ser de 30 m, dejando 5 m como reserva o recortándolos en los puntos de corte indicados.
- El WS2811 controla grupos de 3 LED, por lo que hay aproximadamente 20 píxeles controlables por metro.
- Una tira de 25 m tendrá aproximadamente 500 píxeles controlables.
- Una ventana luminosa de 1 m equivaldrá aproximadamente a 20 píxeles.
- Ambas tiras ejecutarán el barrido de forma sincronizada.

## 3. Componentes

### Control

- 1 NodeMCU ESP32 de 38 pines y USB-C.
- 1 integrado `74AHCT125` o `74HCT125`.
- 1 zócalo DIP-14 para el integrado, opcional.
- 1 step-down de 12 V a 5 V, mínimo 3 A continuos.
- 3 pulsadores normalmente abiertos para `Velocidad +`, `Velocidad -` e `Inicio/Pausa`.
- 1 interruptor basculante ON/OFF de 12 V y 20 A.

### Iluminación

- 2 tiras LED WS2811 RGB de 12 V y 60 LED/m.
- 2 resistencias de entre `330 ohm` y `470 ohm`, una por cada línea de datos.
- 2 capacitores electrolíticos de `1000 uF`, mínimo 16 V y preferentemente 25 V.
- 1 capacitor cerámico de `100 nF` para el `74AHCT125`.

### Alimentación y protección

- 1 batería de motocicleta de 12 V.
- 1 porta-fusible principal.
- 1 fusible principal de `5 A`, 32 VDC.
- 2 porta-fusibles para las tiras.
- 2 fusibles de `3 A`, 32 VDC, uno por tira.
- 1 porta-fusible para la electrónica de control.
- 1 fusible de `1 A`, 32 VDC, para el step-down y el ESP32.
- 1 bornera o distribuidor para `+12 V` y `GND`.
- Aproximadamente 60 m de cable bipolar `2 x 2,5 mm2` para los buses principales.
- Entre 5 m y 10 m de cable bipolar `2 x 1,5 mm2` para las derivaciones.
- Cable par trenzado para cada línea de datos.

Los valores de fusibles son adecuados para una ventana de aproximadamente 1 m encendida por tira. Si se aumenta el brillo o se encienden más segmentos, deben recalcularse.

## 4. Alimentación eléctrica

La conexión general será:

```text
Batería 12 V
    |
    +-- Fusible principal 5 A
            |
            +-- Interruptor general
                    |
                    +-- Fusible 3 A --> Tira LED 1
                    +-- Fusible 3 A --> Tira LED 2
                    +-- Fusible 1 A --> Step-down 12 V a 5 V --> ESP32
```

La alimentación de las tiras se conectará directamente a la línea de 12 V, respetando la polaridad `+12 V` y `GND`.

El ESP32 recibirá 5 V regulados desde el step-down por el pin `5V` de la placa. No se deben conectar los 12 V directamente al ESP32.

Todas las partes deben compartir la misma masa:

```text
GND batería = GND step-down = GND ESP32 = GND 74AHCT125 = GND tiras
```

Si la batería se conecta a un sistema de carga de motocicleta, debe verificarse la tensión máxima, ya que puede superar los 14 V. La tira debe soportar esa tensión o se debe agregar regulación adicional.

## 5. Inyección de alimentación

Para cada tira de 25 m se instalará un bus propio de alimentación. El bus será un par de cables de `+12 V` y `GND` que recorrerá toda la guía.

Los puntos de inyección recomendados estarán aproximadamente en:

```text
0 m ---- 5 m ---- 10 m ---- 15 m ---- 20 m ---- 25 m
```

Esto representa 5 puntos por tira si se alimentan los puntos `0, 5, 10, 15 y 20 m`. Si durante las pruebas se observa una pérdida de brillo en el extremo, se agregará un punto adicional a los 25 m.

La distribución será en paralelo:

```text
Bus +12 V -------------------------------
       |       |       |       |
       +-------+-------+-------+--> puntos de inyección

Bus GND ---------------------------------
       |       |       |       |
       +-------+-------+-------+--> puntos de inyección
```

No hace falta llevar cinco cables independientes desde la batería. Se utiliza un bus principal con derivaciones cortas hacia la tira.

La señal de datos no se inyecta en cada punto. Los datos entran únicamente por `DIN` y recorren la tira siguiendo las flechas de dirección.

## 6. Señales de datos

El ESP32 trabaja con lógica de 3,3 V. El `74AHCT125`, alimentado con 5 V, adapta la señal antes de enviarla a las tiras.

```text
ESP32 GPIO 18 --> 74AHCT125 --> resistencia --> DIN tira 1
ESP32 GPIO 19 --> 74AHCT125 --> resistencia --> DIN tira 2
```

La alimentación del `74AHCT125` será de 5 V y su `GND` deberá estar conectado a la masa común. Los pines de habilitación del integrado deben quedar activos, normalmente conectándolos a `GND`.

Se recomienda colocar el capacitor de `100 nF` entre `VCC` y `GND` del integrado y las resistencias cerca de las entradas `DIN` de las tiras.

## 7. Botones de control

Los tres pulsadores se conectarán entre los GPIO del ESP32 y `GND`:

```text
GPIO 25 --> botón Velocidad + --> GND
GPIO 26 --> botón Velocidad - --> GND
GPIO 27 --> botón Inicio/Pausa --> GND
```

El programa utilizará las resistencias internas `INPUT_PULLUP`, por lo que no son necesarias resistencias externas para los botones.

El interruptor general no se conecta a un GPIO. Su función será cortar la alimentación completa del sistema.

## 8. Lógica del programa

La rutina inicial deberá:

1. Iniciar ambas tiras apagadas.
2. Encender una ventana de aproximadamente 20 píxeles.
3. Desplazar esa ventana desde el inicio hasta el final de las tiras.
4. Apagar la posición anterior antes de avanzar.
5. Repetir el recorrido de forma continua.
6. Mantener las dos tiras sincronizadas.
7. Aumentar el ritmo con `Velocidad +`.
8. Reducir el ritmo con `Velocidad -`.
9. Detener o reanudar el barrido con `Inicio/Pausa`.
10. Aplicar antirrebote a todos los botones.

Si las tiras se recortan a 25 m, el programa utilizará `500` píxeles por tira. Si se mantienen los 30 m completos, utilizará `600` píxeles y limitará el barrido a la zona seleccionada.

## 9. Impermeabilización

La tira comercial es IP20, por lo que el recubrimiento plástico debe proteger completamente:

- La superficie de la tira.
- Las soldaduras.
- Los conectores.
- Los puntos de inyección.
- Los extremos de cada tramo.
- Las uniones entre rollos de 5 m.

La caja de control debe fabricarse preferentemente en PETG o ASA, con tapa, junta y prensaestopas para los cables. Las conexiones deben protegerse con termocontraíble con adhesivo y sellador de silicona neutra.

La batería de motocicleta no debe colocarse dentro de una caja completamente sellada, porque las baterías de plomo pueden liberar gases durante la carga. Debe permanecer en un compartimiento ventilado y separado de la electrónica.

## 10. Secuencia de montaje y pruebas

1. Revisar la polaridad y medir que no existan cortocircuitos.
2. Configurar el step-down a exactamente 5 V sin conectarlo al ESP32.
3. Probar el ESP32 con un tramo corto de tira LED.
4. Verificar el funcionamiento del `74AHCT125` y de las dos líneas de datos.
5. Probar los tres botones y el antirrebote.
6. Probar una tira completa antes de instalarla en la guía.
7. Instalar los buses de alimentación y sus derivaciones.
8. Instalar las inyecciones cada aproximadamente 5 m.
9. Sellar cada conexión y comprobar la continuidad después del sellado.
10. Ejecutar una prueba seca de todo el recorrido.
11. Verificar que no haya pérdida de brillo al final de las guías.
12. Fijar las tiras dentro de las guías flotantes.
13. Realizar una prueba de impermeabilización sin conectar la batería.
14. Repetir la prueba eléctrica completa antes de utilizar el sistema en la piscina.

## 11. Advertencias

- Desconectar la batería antes de modificar el cableado.
- Colocar el fusible principal lo más cerca posible del positivo de la batería.
- No conectar 12 V al ESP32.
- No conectar 5 V a la alimentación de las tiras WS2811 de 12 V.
- Verificar la dirección `DIN` y `DOUT` de las tiras.
- No utilizar cables finos para la alimentación de las tiras.
- Mantener la batería, fusibles y caja de control alejados del agua.
- Probar primero con brillo limitado y colores de bajo consumo.
