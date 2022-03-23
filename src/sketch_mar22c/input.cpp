#include "Arduino.h"
#include <EnableInterrupt.h>

#include "setup.h"
#include "input.h"

#define BOUNCING_TIME 200

int buttons[] = {T1_PIN, T2_PIN, T3_PIN, T4_PIN};
int leds[] = {L1_PIN, L2_PIN, L3_PIN, L4_PIN, LS_PIN};

static int pressedBtn = -1;
/** An array in which in the i-th position there is the last 
 *  time the i-th button was toggled.
 */
static unsigned long lastDebounceTime[BUTTONS_LEN] = {0};

// TODO: flag to disable interrupts when not needed
void buttonPressed(int btn) {
    unsigned long now = millis();
    if (now - lastDebounceTime[btn] > BOUNCING_TIME) {
        printOnConsole("Button pressed: " + String(btn));
        pressedBtn = btn;
        lastDebounceTime[btn] = now;
    }
}

void T1Pressed() {
    buttonPressed(0);    
}

void T2Pressed() {
    buttonPressed(1);
}

void T3Pressed() {
    buttonPressed(2);
}

void T4Pressed() {
    buttonPressed(3);
}

void initSystem() {
    Serial.begin(9600);
    pinMode(T1_PIN, INPUT_PULLUP);
    pinMode(T2_PIN, INPUT_PULLUP);
    pinMode(T3_PIN, INPUT_PULLUP);
    pinMode(T4_PIN, INPUT_PULLUP);
    pinMode(L1_PIN, OUTPUT);
    pinMode(L2_PIN, OUTPUT);
    pinMode(L3_PIN, OUTPUT);
    pinMode(L4_PIN, OUTPUT);
    enableInterrupt(T1_PIN, T1Pressed, FALLING);
    enableInterrupt(T2_PIN, T2Pressed, FALLING);
    enableInterrupt(T3_PIN, T3Pressed, FALLING);
    enableInterrupt(T4_PIN, T4Pressed, FALLING);
}

void printOnConsole(String msg) {
    Serial.println(msg);
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

void fadeLed(int pinLed){
    static int brightness = 0, fadeAmount = 5;
    analogWrite(pinLed, brightness);
    brightness += fadeAmount;
    if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount;
    }
    // Warning this function introduce a delay in the super loop!!
    delay(5);
}

int isButtonPressed() {
    // TODO: comment
    noInterrupts();
    int tmp = pressedBtn;
    pressedBtn = -1;
    interrupts();
    return tmp;
}
