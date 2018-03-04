RGBShades for Particle
=========

This is a port of the RGBShades code that allows it to be ran on the Particle. I forked this to easily show a diff of changes needed.

- Removed buttons.h and messages.h, all of that will be handled over exposed function calls
- Added restFunctions.h for exposed function calls
- Changed some math function names, moved text definitions with global text variables in util
- Removed functions and logic needed for PROGMEM. Particle is STM32 and does not have seperate program and data memory like AVR
- Added bitRead definition, not native to Particle
- Removed seperate text effects
- Removed EEPROM, saving settings on power down wasnt worth the hassle to pretty text ;)
- Added global text variables to util to handle saving constructed strings and style settings

TODO
- Change text styles from function
- Add back EEPROM

=========
Firmware for RGB Shades (Arduino-compatible sunglasses with 16x5 matrix of WS2812B pixels)

Requires FastLED 2.1 or higher, available here: https://github.com/FastLED/FastLED

RGB Shades described here: https://www.kickstarter.com/projects/macetech/rgb-led-shades

When downloading ZIP file, remove "-master" at the end of the folder name before attempting
to open in the Arduino IDE

If your RGB Shades were purchased before July 2015:
This version has the standard Arduino bootloader. R9 and R10 near the control buttons will be present.
Select the “Arduino Pro or Pro Mini” option. Then, go back into the Tools menu and find the Processor option and select “ATmega328 (5V, 16MHz)”.

If your RGB Shades were purchased after July 2015:
This version has the Optiboot bootloader. R9 and 10 near the control buttons will be missing.
Select the “Arduino Mini” option. Then, go back into the Tools menu and find the Processor option and select “ATmega328”.
