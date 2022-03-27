#ifndef __INPUT_H__
#define __INPUT_H__

/**
 * Initialize all the components of the system.
 */
void initSystem();

void enableInterruptsOnButtons();
void disableInterruptsOnButtons();

/**
 * Print on the console the message given in input.
 */
void printOnConsole(String msg);

/**
 * Turns off all the leds of the circuit.
 */
void turnOffLeds();

/**
 * Turns on the led attached to the pin given in input.
 */
void turnOnLed(int pinLed);

/**
 * Turns off the led attached to the pin given in input.
 */
void turnOffLed(int pinLed);

/**
 * Fades in and out the led attached to the pin given in input.
 */
void fadeLed(int pinLed);

/** 
 *  Returns the number of the button pressed or -1 if 
 *  no button have been pressed.
 */
int isButtonPressed();

/**
 * Returns a value in [0, 1023] corresponding to the analog
 * read of the potentiometer attached to the pin given in input.
 */
int readPotentiometer(int pinPot);

void deepSleepSystem();

#endif
