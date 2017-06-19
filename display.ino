/*
   display.ino

   Visual oriented functions for a 'strip'.
 */

void wipeRainbow() {
    fill_rainbow(&(strip[0]), NUM_STRIP_LED, 0);
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
