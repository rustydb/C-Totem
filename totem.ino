#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#include "TimerOne.h"
#include "ClickEncoder.h"

// Used PINs.
#define PIN_NEO_STRIP 6
#define PIN_ONBD_LED 7
#define PIN_NEO_ONBD 8
#define PIN_KNOB_A 9
#define PIN_KNOB_B 10
#define PIN_KNOB_SWITCH 12

// Global Constants.
#define NUM_STRIP_LED 120
#define NUM_STATE_LED 1

// Devices.
CRGB strip[NUM_STRIP_LED];
CRGB state[NUM_STATE_LED];
ClickEncoder *knob;


// Globals
int16_t previousEncoderValue, currentEncoderValue;
uint8_t settingMode, settingValue = 0;
uint8_t brightness, maximumBrightness = 255;
uint8_t minimumBrightness = 0;
uint16_t speed, maximumSpeed = 24;
uint16_t minimumSpeed = 2;
uint16_t colorCounter = 0;
bool direction = true;

void setup() {
    // Start broadcasting on the serial port for debugging.
    Serial.begin(9600);
    // Activate our onboard LED.
    pinMode(PIN_ONBD_LED, OUTPUT);
    // Activate the NeoPixels.
    FastLED.addLeds<NEOPIXEL, PIN_NEO_STRIP>(strip, NUM_STRIP_LED);
    FastLED.addLeds<NEOPIXEL, PIN_NEO_ONBD>(state, NUM_STATE_LED);
    // Activate the encoder knob.
    knob = new ClickEncoder(PIN_KNOB_A, PIN_KNOB_B, PIN_KNOB_SWITCH, 4, false);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerISR);
    previousEncoderValue = 0;
    // General things.
    printSetting();
    changeAnimation();
    Serial.println("Starting program..");
}

void loop() {
    readKnob();
}
