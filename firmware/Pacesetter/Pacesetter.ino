#include <Arduino.h>
#include <FastLED.h>

#include "config.h"

CRGB rightStrip[NUM_PIXELS];
CRGB leftStrip[NUM_PIXELS];

enum class TravelPhase {
    Outbound,
    Return
};

struct DebouncedButton {
    explicit DebouncedButton(uint8_t inputPin) : pin(inputPin) {}

    void begin() {
        pinMode(pin, INPUT_PULLUP);
        lastReading = digitalRead(pin);
        stableState = lastReading;
    }

    bool wasPressed() {
        const bool reading = digitalRead(pin);
        const uint32_t now = millis();

        if (reading != lastReading) {
            lastChangeAt = now;
            lastReading = reading;
        }

        if ((now - lastChangeAt) >= BUTTON_DEBOUNCE_MS && reading != stableState) {
            stableState = reading;
            return stableState == LOW;
        }

        return false;
    }

private:
    uint8_t pin;
    bool lastReading = HIGH;
    bool stableState = HIGH;
    uint32_t lastChangeAt = 0;
};

DebouncedButton speedUpButton(SPEED_UP_BUTTON_PIN);
DebouncedButton speedDownButton(SPEED_DOWN_BUTTON_PIN);
DebouncedButton startPauseButton(START_PAUSE_BUTTON_PIN);

TravelPhase phase = TravelPhase::Outbound;
uint16_t windowStart = 0;
uint32_t stepIntervalMs = DEFAULT_STEP_INTERVAL_MS;
uint32_t lastStepAt = 0;
bool running = false;

const CRGB PACE_COLOR = CRGB::Green;
constexpr uint16_t LAST_WINDOW_START = NUM_PIXELS - WINDOW_PIXELS;

void clearStrips() {
    fill_solid(rightStrip, NUM_PIXELS, CRGB::Black);
    fill_solid(leftStrip, NUM_PIXELS, CRGB::Black);
}

void drawWindow(CRGB* strip) {
    for (uint16_t offset = 0; offset < WINDOW_PIXELS; ++offset) {
        const uint16_t pixel = windowStart + offset;
        if (pixel < NUM_PIXELS) {
            strip[pixel] = PACE_COLOR;
        }
    }
}

void renderFrame() {
    clearStrips();

    if (phase == TravelPhase::Outbound) {
        drawWindow(rightStrip);
    } else {
        drawWindow(leftStrip);
    }

    FastLED.show();
}

void stopAndClear() {
    running = false;
    clearStrips();
    FastLED.show();
}

void toggleRunning() {
    running = !running;
    lastStepAt = millis();

    if (running) {
        renderFrame();
        Serial.println("Barrido iniciado");
    } else {
        stopAndClear();
        Serial.println("Barrido pausado");
    }
}

void increaseSpeed() {
    if (stepIntervalMs > MIN_STEP_INTERVAL_MS) {
        stepIntervalMs = max(MIN_STEP_INTERVAL_MS, stepIntervalMs - SPEED_STEP_MS);
    }

    Serial.print("Intervalo: ");
    Serial.print(stepIntervalMs);
    Serial.println(" ms");
}

void decreaseSpeed() {
    stepIntervalMs = min(MAX_STEP_INTERVAL_MS, stepIntervalMs + SPEED_STEP_MS);

    Serial.print("Intervalo: ");
    Serial.print(stepIntervalMs);
    Serial.println(" ms");
}

void advanceWindow() {
    if (phase == TravelPhase::Outbound) {
        if (windowStart == LAST_WINDOW_START) {
            phase = TravelPhase::Return;
        } else {
            ++windowStart;
        }
    } else if (windowStart == 0) {
        phase = TravelPhase::Outbound;
    } else {
        --windowStart;
    }
}

void handleButtons() {
    if (speedUpButton.wasPressed()) {
        increaseSpeed();
    }

    if (speedDownButton.wasPressed()) {
        decreaseSpeed();
    }

    if (startPauseButton.wasPressed()) {
        toggleRunning();
    }
}

void setup() {
    Serial.begin(115200);

    FastLED.addLeds<WS2811, RIGHT_DATA_PIN, RGB>(rightStrip, NUM_PIXELS);
    FastLED.addLeds<WS2811, LEFT_DATA_PIN, RGB>(leftStrip, NUM_PIXELS);
    FastLED.setBrightness(LED_BRIGHTNESS);
    FastLED.clear(true);

    speedUpButton.begin();
    speedDownButton.begin();
    startPauseButton.begin();

    Serial.println("Pacesetter listo. Presione Inicio/Pausa para comenzar.");
}

void loop() {
    handleButtons();

    if (!running) {
        delay(1);
        return;
    }

    const uint32_t now = millis();
    if ((now - lastStepAt) >= stepIntervalMs) {
        lastStepAt = now;
        renderFrame();
        advanceWindow();
    }
}
