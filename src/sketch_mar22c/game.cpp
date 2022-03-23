#include "Arduino.h"
#include "setup.h"
#include "game.h"
#include "input.h"

#define TIMEOUT_READY 10000

/** Game parameters */
GameStatus gameStatus;
static BallDirection ballDirection = RIGHT;
static int ballPosition = 0;
static int score = 0;
static const int t1 = 5000;
static int t2 = 10000;
static int S = 1000;
static int F = 2;

/** timers variables */
static unsigned long referenceBlink;
static unsigned long referenceBlinkLed;
static unsigned long referencePlay;

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
    static unsigned long reference = millis(); // TO CHECK
    unsigned long now = millis();
    if (now - reference > TIMEOUT_READY) {
        gameStatus = SLEEP;
    } else if (isButtonPressed() == 0) { // T1 has been pressed
        turnOffLeds();
        printOnConsole("Go!");
                
        gameStatus = BLINK;
        referenceBlink = millis();
        referenceBlinkLed = millis();
    } else {
        fadeLed(LS_PIN);
    }
}

void updateDirection() {
    if (ballPosition + ballDirection == LEDS_LEN - 1) {
        ballDirection = LEFT;
    } else if (ballPosition + ballDirection == 0) {
        ballDirection = RIGHT;
    }
}

void gameBlink() {
    if (millis() - referenceBlink <= t1) {
        if (millis() - referenceBlinkLed > S) {
            turnOffLed(leds[ballPosition]);
            updateDirection();
            ballPosition += ballDirection;
            referenceBlinkLed = millis();
        } else {
            turnOnLed(leds[ballPosition]);
        }
    } else {
        gameStatus = PLAY;
        referencePlay = millis();
    }
}

void gamePlay() {
    long btnPressed = isButtonPressed();
    if (millis() - referencePlay <= t2) {
        if (btnPressed == ballPosition) {
            score += 1;
            printOnConsole("> New point! Score:" + String(score));
            ballPosition = 0;
            ballDirection = RIGHT;
            turnOffLeds();

            // params
            t2 = t2 / F;
            S -= 100;

            gameStatus = BLINK;
            referenceBlink = millis();
            referenceBlinkLed = millis();
        } else if (btnPressed != -1 && btnPressed != ballPosition) {
            gameStatus = OVER;
        }
    } else {
        Serial.println("Timeout");
        gameStatus = OVER;
    }
}

void sleep() {
    turnOffLeds();
    printOnConsole("Sleep!");
}

void gameOver() {
    turnOffLeds();
    printOnConsole("GAME OVER!");
}
