/*
   display.ino

   Visual oriented functions for a 'strip'.
 */

void wipeRainbow() {
    fill_rainbow(&(strip[0]), NUM_STRIP_LED, 0);
    FastLED.show();
}

void meteorChaser(uint16_t meteorBodyPixel, uint8_t tailLength, uint16_t fadeValue, bool rainbowTail) {
    int fadeSpectrum = fadeValue;
    int fadeIncrement = (256 - fadeValue) / tailLength;

    if (rainbowTail == true) {
        fill_rainbow(&(strip[meteorBodyPixel]), tailLength, colorCounter);
    } else {
        if (direction == true) {
            for (uint16_t i = meteorBodyPixel; i > (meteorBodyPixel - tailLength); i--) {
                strip[i].setHue(colorCounter);
                strip[i].fadeLightBy(fadeSpectrum);
                fadeSpectrum += fadeIncrement;
            }
        } else {
            for (uint16_t i = (meteorBodyPixel - tailLength); i < meteorBodyPixel; i++) {
                strip[i].setHue(colorCounter);
                strip[i].fadeLightBy(fadeSpectrum);
                fadeSpectrum += fadeIncrement;
            }
        }
    }
    FastLED.show();
}


// Toggle the onboardLED.
void onboardLED(bool lit) {
    if (lit == true) {
        digitalWrite(PIN_ONBD_LED, HIGH);
    } else {
        digitalWrite(PIN_ONBD_LED, LOW);
    }
}
