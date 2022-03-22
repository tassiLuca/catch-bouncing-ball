#include "Arduino.h"
#include "setup.h"
#include "game.h"
#include "input.h"

#define TIMEOUT_READY 10000

GameStatus gameStatus;

static String welcomeMsg = 
    "************************************************ \n"
    "Welcome to the Catch the Bouncing Led Ball Game. \n"
    "Press Key T1 to Start. \n"
    "************************************************ \n";

void welcome() {
    turnOffLeds();
    printOnConsole(welcomeMsg);
    gameStatus = READY;
}

void gameReady() {
    static unsigned long reference = millis();
    unsigned long now = millis();
    if (now - reference > TIMEOUT_READY) {
        gameStatus = SLEEP;
    } else {
        fadeLed(LS_PIN);
    }
}

void sleep() {
    turnOffLeds();
    Serial.println("SLEEP");
}
