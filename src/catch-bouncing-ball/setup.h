#ifndef __SETUP_H__
#define __SETUP_H__

// When the following is defined logging messages are print on the console line.
#define DBG 1

/**
 * Led pins.
 */
#define L1_PIN 2
#define L2_PIN 3
#define L3_PIN 4
#define L4_PIN 5
#define LS_PIN 6

/**
 * Buttons and potentiometer pins.
 */
#define POT_PIN A0
#define T1_PIN 8
#define T2_PIN 9
#define T3_PIN 10
#define T4_PIN 11

/**
 * An array which contains all the buttons pins.
 */
extern int buttons[];

/**
 * An array which contains all the led pins.
 */
extern int leds[];

/**
 * The number of buttons.
 */
#define BUTTONS_LEN 4

/**
 * The number of leds.
 */
#define LEDS_LEN 5

#endif
