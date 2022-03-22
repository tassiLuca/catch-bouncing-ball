#ifndef __INPUT_H__
#define __INPUT_H__

/**
 * Initialize all the components of the system.
 */
void initSystem();

/**
 * Print on the console the message given in input.
 */
void printOnConsole(String msg);

/**
 * Lights off all the leds.
 */
void turnOffLeds();

/**
 * Fades in and out a led repeatedly.
 */
void fadeLed(int pinLed);

/** 
 *  Returns the number of the button pressed or -1 if 
 *  no button have been pressed.
 */
bool isButtonPressed();

#endif
