#include "Arduino.h"
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include <avr/sleep.h>

#include "setup.h"
#include "input.h"

// TODO LIST
// - rename this file and the header one in boundary.h/c

/**
 * The debounce time. This value is hardware-dependent and must be 
 * adjusted according to the buttons quality.
 */
#define BOUNCING_TIME 200

int buttons[] = {T1_PIN, T2_PIN, T3_PIN, T4_PIN};
int leds[]    = {L1_PIN, L2_PIN, L3_PIN, L4_PIN, LS_PIN};

/**
 * A variable with the position in the array of the corresponding 
 * led which has been pressed.
 */
static int pressedBtn = -1;
/** 
 *  An array in which, in the i-th position, there is the last time
 *  the i-th button was toggled. This is useful for bouncing problems.
 */
static unsigned long lastDebounceTime[BUTTONS_LEN] = {0};

/** 
 *  Returns the index in the array of the button pin given in input.
 */
static int getIndexOfButton(int btnPin) {
    for (int i = 0; i < BUTTONS_LEN; i++) {
        if (buttons[i] == btnPin) {
            return i;        
        }
    }
    return -1;
}

/**
 * Buttons handler routing activated from the buttons interrupt.
 */
// TODO: flag to disable in soft way interrupts when not needed or disable it via detachInterrupt(): 
// see [https://www.arduino.cc/reference/it/language/functions/external-interrupts/detachinterrupt/]
static void buttonPressed() {
    pressedBtn = getIndexOfButton(arduinoInterruptedPin);
    unsigned long now = millis();
    if (pressedBtn != -1 && now - lastDebounceTime[pressedBtn] > BOUNCING_TIME) {
        lastDebounceTime[pressedBtn] = now;
#ifdef DBG
        printOnConsole("Button pressed: " + String(pressedBtn));
#endif
    }
}

void initSystem() {
    Serial.begin(9600);
    // [NOTE] Keep in mind the pull-up means the pushbutton's logic is inverted:
    //        it goes HIGH when it's open, and LOW when it's pressed.
    //        see [https://docs.arduino.cc/tutorials/generic/digital-input-pullup]
    for (int i = 0; i < BUTTONS_LEN; i++) {
        pinMode(buttons[i], INPUT_PULLUP);
        enableInterrupt(buttons[i], buttonPressed, FALLING);
    }
    for (int i = 0; i < LEDS_LEN; i++) {
        pinMode(leds[i], OUTPUT);
    }
#ifdef DBG
    printOnConsole(">>> ACTIVE THE DEBUG MODE: MAX LEVEL OF VERBOSITY <<<");
#endif
}

int readPotentiometer(int pinPot) {
    return analogRead(pinPot);
}

void printOnConsole(String msg) {
    Serial.println(msg);
    Serial.flush();
}

void turnOffLeds() {
    for (int i = 0; i < LEDS_LEN; i++) {
        digitalWrite(leds[i], LOW);
    }
}

void turnOnLed(int pinLed) {
    digitalWrite(pinLed, HIGH);
}

void turnOffLed(int pinLed) {
    digitalWrite(pinLed, LOW);
}

void fadeLed(int pinLed) {
    static int brightness = 0, fadeAmount = 5;
    analogWrite(pinLed, brightness);
    brightness += fadeAmount;
    if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount;
    }
    // Warning this function introduce a litte delay in the super loop!!
    delay(5);
}

int isButtonPressed() {
    // [NOTE] To avoid race conditions disable interrupts.
    noInterrupts();
    int tmp = pressedBtn;
    pressedBtn = -1;
    interrupts();
    return tmp;
}

void deepSleepSystem() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
}
