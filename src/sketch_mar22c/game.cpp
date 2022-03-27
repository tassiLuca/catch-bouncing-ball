#include "Arduino.h"
#include "setup.h"
#include "game.h"
#include "input.h"

/** The fixed amount of time which the user has to begin 
 *  the play game before the system goes in deep sleeping. */
#define TIMEOUT_READY 10000
/** The following are the minimum and maximum value of random 
 *  time T1 in which the ball moves repeatedly back and forth. */
#define RAND_MIN_TIME 3
#define RAND_MAX_TIME 10
/**  The number of levels difficulties. */
#define LEVELS 8

#define WELCOME_MSG "************************************************ \n" \
                    "Welcome to the Catch the Bouncing Led Ball Game. \n" \
                    "Press Key T1 to Start. \n"                           \
                    "************************************************ \n"
/** An enum describing the two possible directions where the ball can moves. */
typedef enum {
    RIGHT = 1,
    LEFT = -1
} BallDirection;

/** The current game status. */
GameStatus gameStatus;
/** The current direction of the ball towards which it is moving. */
static BallDirection ballDirection;
/** The speed of the ball. */
static int ballSpeed;
/** The current position of the ball (actually, the led ton which is on). */
static int ballPosition;
/** The current difficulty level. */
static int level;
/** The actual game score. */
static int score;
/** The amount of time in which the ball moves repeatedly back and forth. */
static int t1;
/** The amount of time within which the user must press 
 *  a button before the game ends. */
static int t2;
/** The factor to reduce T2 for each turn of play. */
static float F;

/** Timers variables */
static unsigned long referenceReady;
static unsigned long referenceBlink;
static unsigned long referenceBlinkLed;
static unsigned long referencePlay;

/**
 * Re-maps a number from one range to another, like map() does but returning a 
 * floating value.
 */
static float mapfloat(long x, long fromLow, long fromHigh, float toLow, float toHigh) {
  return (float)(x - fromLow) * (toHigh - toLow) / (float)(fromHigh - fromLow) + toLow;
}

/**
 * Updates the game parameters according to the current game status: if it
 * is "PLAY" the parameters are updated incrementally, otherwise are reset.
 */
static void updateGameParameters() {
    ballPosition = 0;
    ballDirection = RIGHT;
    t1 = ((random() % RAND_MAX_TIME) + RAND_MIN_TIME) * 1000;
    if (gameStatus == PLAY) {
        t2 = max(t2 / F, 30);
        ballSpeed = max(ballSpeed - 150, 30);
        score += 1;
    } else {
        t2 = 10000;
        ballSpeed = 1000;
        score = 0;
        F = mapfloat(level, 1, LEVELS, 1.1, 3);
    }
#ifdef DBG
    printOnConsole("You have " + String(t2) + "ms to push the button");
#endif
}

void welcome() {
    turnOffLeds();
    printOnConsole(WELCOME_MSG);
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
        disableInterruptsOnButtons();
        referenceBlink = millis();
        referenceBlinkLed = millis();
        updateGameParameters();
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
        if (millis() - referenceBlinkLed > ballSpeed) {
            turnOffLed(leds[ballPosition]);
            updateDirection();
            ballPosition += ballDirection;
            referenceBlinkLed = millis();
        } else {
            turnOnLed(leds[ballPosition]);
        }
    } else {
        gameStatus = PLAY;
        enableInterruptsOnButtons();
        referencePlay = millis();
    }
}

void gamePlay() {
    long btnPressed = isButtonPressed();
    if (millis() - referencePlay > t2 || (btnPressed != -1 && btnPressed != ballPosition)) {
        gameStatus = OVER;
    } else if (btnPressed == ballPosition) {
        updateGameParameters();
        printOnConsole("New point! Score:" + String(score));
        turnOffLeds();
        gameStatus = BLINK;
        disableInterruptsOnButtons();
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
