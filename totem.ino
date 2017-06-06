#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define STRIP_NEO_PIN 6
#define ONBOARD_LED 7
#define ONBOARD_NEO_PIN 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(61, STRIP_NEO_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel onboard = Adafruit_NeoPixel(1, ONBOARD_NEO_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel currentPixel = Adafruit_NeoPixel();
int led_voltage = HIGH;

void setup() {
    // Onboard NeoPixel.
    onboard.begin();
    onboard.show();
    // Attached NeoPixels.
    strip.begin();
    strip.show();
    // Onboard LED.
    pinMode(ONBOARD_LED, OUTPUT);
}

void loop() {
    onboardNeoPixel();
    attachedNeoPixels();
    onboardLED();
}

void onboardNeoPixel() {
    currentPixel = onboard;
    rainbowCycle(0);
}

void attachedNeoPixels() {
    currentPixel = strip;
    rainbowCycle(20);
    rainbow(20);
    theaterChaseRainbow(50);
}

void onboardLED() {
    digitalWrite(ONBOARD_LED, led_voltage);
    if (led_voltage == HIGH) {
        led_voltage = LOW;
    } else {
        led_voltage = HIGH;
    }
}

// Fill the dots one after the other with a color.
void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i = 0; i < currentPixel.numPixels(); i++) {
        currentPixel.setPixelColor(i, c);
        currentPixel.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;
    for(j = 0; j < 256; j++) {
        for(i = 0; i < currentPixel.numPixels(); i++) {
            currentPixel.setPixelColor(i, Wheel((i + j) & 255));
        }
        currentPixel.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout.
void rainbowCycle(uint8_t wait) {
    uint16_t i, j;
    for(j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel.
        for(i = 0; i < currentPixel.numPixels(); i++) {
            currentPixel.setPixelColor(i, Wheel(((i * 256 / currentPixel.numPixels()) + j) & 255));
        }
        currentPixel.show();
        delay(wait);
    }
}

// Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
    for (int j = 0; j < 10; j++) { //do 10 cycles of chasing.
        for (int q = 0; q < 3; q++) {
            for (uint16_t i=0; i < currentPixel.numPixels(); i = i + 3) {
                currentPixel.setPixelColor(i + q, c); //turn every third currentPixel on.
            }
            currentPixel.show();
            delay(wait);
            for (uint16_t i = 0; i < currentPixel.numPixels(); i = i + 3) {
                currentPixel.setPixelColor(i + q, 0); //turn every third currentPixel off.
            }
        }
    }
}

// Theatre-style crawling lights with rainbow effect.
void theaterChaseRainbow(uint8_t wait) {
    for (int j = 0; j < 256; j++) { // cycle all 256 colors in the wheel.
        for (int q = 0; q < 3; q++) {
            for (uint16_t i = 0; i < currentPixel.numPixels(); i = i + 3) {
                currentPixel.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third currentPixel on.
            }
            currentPixel.show();
            delay(wait);
            for (uint16_t i = 0; i < currentPixel.numPixels(); i = i + 3) {
                currentPixel.setPixelColor(i + q, 0); //turn every third currentPixel off.
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return currentPixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return currentPixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return currentPixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
