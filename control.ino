/*
   control.ino

   Input oriented functions for a 'strip'.
 */
static char const* settingModes[] = {"Pattern", "Speed", "Brightness"};
static uint32_t settingColors[] = {0xff005f, 0x00ff00, 0xd75f00};

// Update the mode.
void updateMode() {
    // Set this to number of cases in applyKnob().
    if (settingMode < 2) {
        settingMode++;
    } else {
        settingMode = 0;
    }
    currentEncoderValue = previousEncoderValue = 0;
    printSetting();

}

// Update the animation setting value.
void updateValue() {
    if (currentEncoderValue > previousEncoderValue) {
        if (settingValue >= 254) {
            settingValue = 10;
        } else {
            settingValue++;
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        if (settingValue > 11) {
            settingValue = 0;
        } else {
            settingValue--;
        }
    }
}

// Knob interrupt events.
void readKnob() {
    currentEncoderValue     += knob->getValue();
    ClickEncoder::Button b   = knob->getButton();
    if (b != ClickEncoder::Open) {
        switch (b) {
            case ClickEncoder::Clicked:
                updateMode();
                break;
            case ClickEncoder::Released:
                onboardLED(false);
                break;
            case ClickEncoder::Held:
                onboardLED(true);
                break;
            default:
                ;
        }
    }
    if (b == ClickEncoder::Open) {
        updateValue();
        if (currentEncoderValue != previousEncoderValue) {
            applyKnob();
        }
    }
    FastLED.show();
}

// Applies the knob's currentEncoderValue to the strip.
void applyKnob() {
    /* Legend:
       0 - Change Pattern
       1 - Set Speed
       2 - Set Brightness
     */
    switch (settingMode) {
        case 0:
            changeAnimation();
            break;
        case 1:
            setSpeed();
            break;
        case 2:
            //Changes the "brightness" value of the whole strip. Not exactly what you'd think...
            setBrightness();
            break;
        default:
            ;
    }
    previousEncoderValue = currentEncoderValue;
}

// Changes the animation of the strip.
void changeAnimation() {
    /* Legend:
       0 - shift
       1 - shift
       2 - shift
       3 - shift
       4 - shift
       5 - cycleSolid
     */
    switch (settingValue) {
        case 0:
            wipeRainbow();
            //      fill_solid(&(infinity[0]), 60, CRGB::Black);
            //      meteorChaser(15, 12, 160, false);
            break;
        case 1:
            //      if (settingMode == 0) {
            //        fill_solid(&(infinity[0]), 60, CRGB::Black);
            //      }
            //      fourPoints(7, 21, 35, 49);
            break;
        case 2:
            //      colorCounter = random8();
            //      fillSolid(colorCounter);
            //      colorCounter = random8();
            //      meteorChaser(20, 14, 160, true);
            break;
        case 3:
            //      fillSolid(colorCounter);
            //      meteorChaser(30, 26, 160, true);
            break;
        case 4:
            //      wipeInfinity(25);
            break;
        case 5:
            //      fillSolid(colorCounter);
            break;
        default:
            break;
    }
    previousEncoderValue = currentEncoderValue;
}

// Sets the speed of the pattern from knob values.
void setSpeed() {
    // Set a minimum and maximum.
    if (currentEncoderValue > previousEncoderValue) {
        if (speed > minimumSpeed) {
            Serial.print("Slow down to ");
            speed -= 2;
            Serial.println(speed);
        } else {
            Serial.print("At minimum speed: ");
            Serial.println(minimumSpeed);
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        if (speed < maximumSpeed) {
            Serial.print("Speed up to ");
            speed += 2;
            Serial.println(speed);
        } else {
            Serial.print("At maximum speed: ");
            Serial.println(maximumSpeed);
        }
    }
}

// Brightens or dims the strip from knob values.
void setBrightness() {
    if (currentEncoderValue > previousEncoderValue) {
        if (brightness > 250) {
            Serial.print("At maximum brightness: ");
            Serial.println(maximumBrightness);
        } else {
            Serial.print("Brighten to ");
            brightness += 5;
            Serial.println(brightness);
            for (uint16_t i = 0; i < NUM_STRIP_LED; i++) {
                strip[i].addToRGB(5);
//                strip[i] += 5;
            }
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        if (brightness <= 0) {
            Serial.print("At minimum brightness: ");
            Serial.println(minimumBrightness);
        } else {
            Serial.print("Dim to ");
            brightness -= 5;
            Serial.println(brightness);
            for (uint16_t i = 0; i < NUM_STRIP_LED; i++) {
                strip[i].subtractFromRGB(5);
//                strip[i] -= 5;
            }
        }
    }
    FastLED.show();
}

// Interrupt for encoder knob.
void timerISR() {
    knob->service();
}

// Print the setting using the global list.
void printSetting() {
    Serial.print("Configuring: ");
    Serial.println(settingModes[settingMode]);
    fill_solid(&(state[0]), NUM_STATE_LED, CRGB(settingColors[settingMode]));
    for (uint16_t i = 0; i < NUM_STATE_LED; i++) {
        state[i] %= 20;
    }
    FastLED.show();
}
