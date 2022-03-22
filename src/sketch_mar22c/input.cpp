#include "Arduino.h"
#include <EnableInterrupt.h>

#include "setup.h"
#include "input.h"

int buttons[] = {L1_PIN, L2_PIN, L3_PIN, L4_PIN, LS_PIN};

static int brightness = 0;
static int fadeAmount = 5;

void interruptFunction() {
    Serial.println("HELLO INTERRUPT");
}

void initSystem() {
    Serial.begin(9600);
    pinMode(T1_PIN, INPUT_PULLUP);
    pinMode(T2_PIN, INPUT_PULLUP);
    pinMode(T3_PIN, INPUT_PULLUP);
    pinMode(T4_PIN, INPUT_PULLUP);
    enableInterrupt(T1_PIN, interruptFunction, FALLING);
}

void printOnConsole(String msg) {
    Serial.println(msg);
}

void turnOffLeds() {
    int buttonsSize = sizeof(buttons) / sizeof(buttons[0]);
    for (int i = 0; i <= buttonsSize; i++) {
        digitalWrite(buttons[i], LOW);
    }
}

void fadeLed(int pinLed){
    analogWrite(pinLed, brightness);
    brightness += fadeAmount;
    if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount;
    }
    delay(5);
}
