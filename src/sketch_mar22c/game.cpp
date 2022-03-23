#include "Arduino.h"
#include "setup.h"
#include "game.h"
#include "input.h"

#define TIMEOUT_READY 10000

static int score = 0;

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
    } else if (isButtonPressed() == 0) { // T1 has been pressed
        gameStatus = BLINK;
        printOnConsole("Go!");
    } else {
        fadeLed(LS_PIN);
    }
}

void gameBlink() {
    turnOffLeds();
}

void gamePlay() {
    turnOffLeds();
}

void sleep() {
    turnOffLeds();
    printOnConsole("Sleep!");
}
