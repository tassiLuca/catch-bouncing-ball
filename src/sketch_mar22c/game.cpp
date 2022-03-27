#include "Arduino.h"
#include "setup.h"
#include "game.h"
#include "input.h"

#define TIMEOUT_READY 10000
#define RAND_MIN_TIME 3
#define RAND_MAX_TIME 10
#define LEVELS 8

typedef enum {
    RIGHT = 1,
    LEFT = -1
} BallDirection;

/* Game parameters */
GameStatus gameStatus;
static BallDirection ballDirection;
static int ballPosition;
static int level;
static int score;
static int t1;
static int t2;
static int S;
static float F;

/* Timers variables */
static unsigned long referenceReady;
static unsigned long referenceBlink;
static unsigned long referenceBlinkLed;
static unsigned long referencePlay;

static float mapfloat(long x, long in_min, long in_max, float out_min, float out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

static void resetGameParameters() {
    score = 0;
    t1 = ((random() % RAND_MAX_TIME) + RAND_MIN_TIME) * 1000;
    t2 = 10000;
    S = 1000;
    F = mapfloat(level, 1, LEVELS, 1.1, 3);
    ballPosition = 0;
    ballDirection = RIGHT;
#ifdef DBG
    printOnConsole("The difficulty level is " + String(level));
    printOnConsole("The factor F is " + String(F));
#endif
}

static void updateGameParameters() {
    score += 1;
    t1 = ((random() % RAND_MAX_TIME) + RAND_MIN_TIME) * 1000;
    t2 = max(t2 / F, 30);
    S = max(S - 150, 30); 
#ifdef DBG
    printOnConsole("The speed of the led is " + String(S));
    printOnConsole("You have " + String(t2) + " ms to push a button");
#endif
    ballPosition = 0;
    ballDirection = RIGHT;
}

void welcome() {
    const String welcomeMsg = 
        "************************************************ \n"
        "Welcome to the Catch the Bouncing Led Ball Game. \n"
        "Press Key T1 to Start. \n"
        "************************************************ \n";
    turnOffLeds();
    printOnConsole(welcomeMsg);
    gameStatus = READY;
    referenceReady = millis();
}

void gameReady() {
    if (millis() - referenceReady > TIMEOUT_READY) {
        gameStatus = SLEEP;
    } else if (isButtonPressed() == 0) { // T1 has been pressed => TODO: improve
        turnOffLeds();
        printOnConsole("Go!");
        gameStatus = BLINK;
        referenceBlink = millis();
        referenceBlinkLed = millis();
        resetGameParameters();
    } else {
        fadeLed(LS_PIN);
        level = map(readPotentiometer(POT_PIN), 0, 1023, 1, LEVELS);
        //printOnConsole("Level: " + String(level));    // TODO: ASCII art
    }
}

void updateDirection() {
    if (ballPosition + ballDirection >= LEDS_LEN - 1) {
        ballDirection = LEFT;
    } else if (ballPosition + ballDirection < 0) {
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
    if (millis() - referencePlay > t2 || (btnPressed != -1 && btnPressed != ballPosition)) {
        gameStatus = OVER;
    } else if (btnPressed == ballPosition) {
        printOnConsole("New point! Score:" + String(score));
        turnOffLeds();
        updateGameParameters();
        gameStatus = BLINK;
        referenceBlink = millis();
        referenceBlinkLed = millis();
    }
}

void sleep() {
    turnOffLeds();
    printOnConsole("Sleep!");
    deepSleepSystem();
    gameStatus = SETUP;
}

void gameOver() {
    turnOffLeds();
    printOnConsole("Game Over. Final Score: " + String(score));
    delay(10000);   // TODO avoid magic numbers
    gameStatus = SETUP;
}
