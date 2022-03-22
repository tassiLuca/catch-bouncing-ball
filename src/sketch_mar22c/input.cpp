#include "Arduino.h"
#include <EnableInterrupt.h>

#include "setup.h"
#include "input.h"

#define BUTTONS_LEN (sizeof(buttons) / sizeof(buttons[0]))
int buttons[] = {L1_PIN, L2_PIN, L3_PIN, L4_PIN, LS_PIN};
static int buttonsState[] = {LOW, LOW, LOW, LOW, LOW};

static int brightness = 0;
static int fadeAmount = 5;

// TODO: manage balance problem
void buttonPressed(int buttonPressed) {
    buttonsState[buttonPressed] = HIGH;
    printOnConsole("Stato " + buttonsState[buttonPressed]);
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
    enableInterrupt(T1_PIN, T1Pressed, FALLING);
    enableInterrupt(T2_PIN, T2Pressed, FALLING);
    enableInterrupt(T3_PIN, T3Pressed, FALLING);
    enableInterrupt(T4_PIN, T4Pressed, FALLING);
}

void printOnConsole(String msg) {
    Serial.println(msg);
}

void turnOffLeds() {
    for (int i = 0; i <= BUTTONS_LEN; i++) {
        digitalWrite(buttons[i], LOW);
    }
}

void fadeLed(int pinLed){
    analogWrite(pinLed, brightness);
    brightness += fadeAmount;
    if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount;
    }
    // Warning this function introduce a delay in the super loop!!
    delay(5);
}

bool isButtonPressed() {
    // **THEORETICALLY**
    // In this way if there have been multiple pressions and a check 
    // has not been performed this function returns only the first pression!
    for (int i = 0; i < BUTTONS_LEN; i++) {
        // TODO: disable interrupts to avoid race conditions!
        if (buttonsState[i] == HIGH) {
            buttonsState[i] = LOW;
            return i;
        }
    }
    return -1;
}
