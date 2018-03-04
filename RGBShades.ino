//  Code ported by Kristina Durivage, none of the non Particle was written by me 
//  Original code available at https://github.com/macetech/RGBShades 
//
//   RGB Shades Demo Code
//   Copyright (c) 2015 macetech LLC
//   This software is provided under the MIT License (see license.txt)
//   Special credit to Mark Kriegsman for XY mapping code
//
//   Use Version 3.0 or later https://github.com/FastLED/FastLED
//   ZIP file https://github.com/FastLED/FastLED/archive/master.zip
//
//   Use Arduino IDE 1.0 or later
//
//   If your RGB Shades were purchased before July 2015:
//     This version has the standard Arduino bootloader. R9 and R10 near the control buttons will be present.
//     Select the “Arduino Pro or Pro Mini” option.
//     Then, go back into the Tools menu and find the Processor option and select “ATmega328 (5V, 16MHz)”.
//
//   If your RGB Shades were purchased after July 2015:
//     This version has the Optiboot bootloader. R9 and 10 near the control buttons will be missing.
//     Select the “Arduino Mini” option.
//     Then, go back into the Tools menu and find the Processor option and select “ATmega328”.
//
//   [Press] the SW1 button to cycle through available effects
//   [Press and hold] the SW1 button (one second) to switch between auto and manual mode
//     * Auto Mode (one blue blink): Effects automatically cycle over time
//     * Manual Mode (two red blinks): Effects must be selected manually with SW1 button
//
//   [Press] the SW2 button to cycle through available brightness levels
//   [Press and hold] the SW2 button (one second) to reset brightness to startup value
//
//   Brightness, selected effect, and auto-cycle are saved in EEPROM after a delay
//   The RGB Shades will automatically start up with the last-selected settings


// RGB Shades data output to LEDs is on pin 5
#define LED_PIN  5

// RGB Shades color order (Green/Red/Blue)
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

// Global maximum brightness value, maximum 255
#define MAXBRIGHTNESS 72
#define STARTBRIGHTNESS 102

// Cycle time (milliseconds between pattern changes)
#define cycleTime 15000

// Hue time (milliseconds between hue increments)
#define hueTime 30

#define bitRead(value, bit) (((value) >> (bit)) & 0x01) // needed for text effect

// Include FastLED library and other useful files
#include "Particle.h"
#include "math.h"
#include <FastLED.h>
FASTLED_USING_NAMESPACE;
#include "font.h"
#include "XYmap.h"
#include "utils.h"
#include "effects.h"
#include "restFunctions.h"


// list of functions that will be displayed
// to prevent needing to loop over this function, leave scrollText last
functionList effectList[] = {threeSine,
                             threeDee,
                             plasma,
                             confetti,
                             rider,
                             glitter,
                             slantBars,
                             colorFill,
                             sideRain,
                             scrollText
                            };

const byte numEffects = (sizeof(effectList)/sizeof(effectList[0]));

// Runs one time at the start of the program (power up or reset)
void setup() {
  // write FastLED configuration data
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LAST_VISIBLE_LED + 1);

  // set global brightness value
  FastLED.setBrightness( scale8(currentBrightness, MAXBRIGHTNESS) );
  
  Particle.function("incBright", increaseBrightness);
  Particle.function("rstBright", resetBrightness);
  Particle.function("nextMode", nextMode);
  Particle.function("autoMode", autoCycleMode);
  Particle.function("buildString", buildString);
  Particle.function("buildDirect", buildShortString);
  Particle.function("textMode", switchToTextMode);
}


// Runs over and over until power off or reset
void loop()
{
  currentMillis = millis(); // save the current timer value
  
  // switch to a new effect every cycleTime milliseconds
  if (currentMillis - cycleMillis > cycleTime && autoCycle == true) {
    cycleMillis = currentMillis;
    if (++currentEffect >= numEffects) currentEffect = 0; // loop to start of effect list
    effectInit = false; // trigger effect initialization when new effect is selected
  }

  // increment the global hue value every hueTime milliseconds
  if (currentMillis - hueMillis > hueTime) {
    hueMillis = currentMillis;
    hueCycle(1); // increment the global hue value
  }

  // run the currently selected effect every effectDelay milliseconds
  if (currentMillis - effectMillis > effectDelay) {
    effectMillis = currentMillis;
    effectList[currentEffect](); // run the selected effect function
    random16_add_entropy(1); // make the random values a bit more random-ish
  }

  // run a fade effect too if the confetti effect is running
  if (effectList[currentEffect] == confetti) fadeAll(1);

  FastLED.show(); // send the contents of the led memory to the LEDs
}