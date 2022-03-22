#include "Arduino.h"
#include "setup.h"
#include "input.h"

void initSystem() {
    Serial.begin(9600);
    pinMode(T1_PIN, INPUT_PULLUP);
    pinMode(T2_PIN, INPUT_PULLUP);
    pinMode(T3_PIN, INPUT_PULLUP);
    pinMode(T4_PIN, INPUT_PULLUP);
}

void printOnConsole(String msg) {
    Serial.println(msg);
}

void fadeLed(int ledPosition){
    
}
