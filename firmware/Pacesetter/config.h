#pragma once

#include <Arduino.h>

// Hardware configuration
constexpr uint8_t RIGHT_DATA_PIN = 18;
constexpr uint8_t LEFT_DATA_PIN = 19;
constexpr uint8_t SPEED_UP_BUTTON_PIN = 25;
constexpr uint8_t SPEED_DOWN_BUTTON_PIN = 26;
constexpr uint8_t START_PAUSE_BUTTON_PIN = 27;

// A 25 m strip has approximately 500 WS2811-controlled pixels.
constexpr uint16_t NUM_PIXELS = 500;
constexpr uint16_t WINDOW_PIXELS = 20;

// The interval is the time, in milliseconds, between window movements.
constexpr uint32_t DEFAULT_STEP_INTERVAL_MS = 50;
constexpr uint32_t MIN_STEP_INTERVAL_MS = 10;
constexpr uint32_t MAX_STEP_INTERVAL_MS = 200;
constexpr uint32_t SPEED_STEP_MS = 10;
constexpr uint32_t BUTTON_DEBOUNCE_MS = 35;

// Keep the initial brightness conservative while testing the power system.
constexpr uint8_t LED_BRIGHTNESS = 64;
