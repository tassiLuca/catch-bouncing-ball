#ifndef __SETUP_H__
#define __SETUP_H__

#define L1_PIN 2
#define L2_PIN 3
#define L3_PIN 4
#define L4_PIN 5
#define LS_PIN 6

#define POT_PIN A0
#define T1_PIN 8
#define T2_PIN 9
#define T3_PIN 10
#define T4_PIN 11

extern int buttons[];
extern int leds[];
// sizeof doesn't work TODO
#define BUTTONS_LEN 4
#define LEDS_LEN 5

#endif
