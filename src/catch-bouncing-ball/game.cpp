#include "Arduino.h"
#include <math.h>
#include "setup.h"
#include "game.h"
#include "boundary.h"

#define REDUCE_FACTOR(x) log((x))

/** The fixed amount of time which the user has to begin the play game before 
 *  the system goes in deep sleeping and the one after the game over to re-start it. */
#define TIMEOUT 10000
/** The following are the minimum and maximum value of random 
 *  time T1 in which the ball moves repeatedly back and forth. */
#define RAND_MIN_TIME 2
#define RAND_MAX_TIME 8
/** The number of levels difficulties. */
#define LEVELS 8
/** The default ball starting position. */
#define STARTING_BALL_POS 0
/** The minimum ball speed the ball can reach and the default one. */
#define MIN_BALL_SPEED 30
#define DEFAULT_BALL_SPEED 1000
/** The minimum and maximum values for the reduce factor. */
#define MIN_REDUCE_FACTOR 1.1
#define MAX_REDUCE_FACTOR 2.0
/** The default amount of time for t2. */
#define DEFAULT_T2 10000
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
/** The speed of the ball (actually, the time in which each led remains on). */
static int ballSpeed;
/** The current position of the ball (actually, the led ton which is on). */
static int ballPosition;
/** The current difficulty level. */
static int level;
/** The actual game score. */
static int score;
/** The amount of time, in ms, in which the ball moves repeatedly back and forth. */
static int t1;
/** The amount of time, in ms, within which the user must press a button before the game ends. */
static int t2;
/** The factor to reduce T2 for each turn of play. */
static float reduceFactor;

/** Timers variables */
static unsigned long reference;
static unsigned long referenceBlinkLed;

/**
 * Re-maps a number from one range to another, like map() does but returning a 
 * floating value.
 */
static float mapfloat(long x, long fromLow, long fromHigh, float toLow, float toHigh) {
  return (float)(x - fromLow) * (toHigh - toLow) / (float)(fromHigh - fromLow) + toLow;
}

static void changeGameStatus(GameStatus newStatus) {
    gameStatus = newStatus;
    reference = millis();
    referenceBlinkLed = millis();
}

/**
 * Updates the game parameters according to the current game status: if it
 * is "PLAY" the parameters are updated incrementally, otherwise are reset.
 */
static void updateGameParameters() {
    ballPosition = STARTING_BALL_POS;
    ballDirection = RIGHT;
    // NOTE: t1 is measured in ms.
    t1 = ((random() % RAND_MAX_TIME) + RAND_MIN_TIME) * 1000;
    if (gameStatus == PLAY) {   // update the parameters
        t2 = max(t2 / reduceFactor, 0);
        ballSpeed = max(ballSpeed / REDUCE_FACTOR(level), MIN_BALL_SPEED);
        score += 1;
    } else {    // reset the paramaters
        t2 = DEFAULT_T2;
        ballSpeed = DEFAULT_BALL_SPEED;
        score = 0;
        reduceFactor = mapfloat(level, 1, LEVELS, MIN_REDUCE_FACTOR, MAX_REDUCE_FACTOR);
    }
#ifdef DBG
    printOnConsole("You have " + String(t2) + "ms to push the correct button");
#endif
}

void welcome() {
    turnOffLeds();
    printOnConsole(WELCOME_MSG);
    gameStatus = READY;
    reference = millis();
}

void gameReady() {
    if (millis() - reference > TIMEOUT) {
        gameStatus = SLEEP;
    } else if (getButtonPressed() == 0) {
        turnOffLeds();
        printOnConsole("Go!");
        changeGameStatus(BLINK);
        disableInterruptsOnButtons();
        updateGameParameters();
    } else {
        fadeLed(LS_PIN);
        // TO DO BETTER
        int tmp = level;
        level = map(readPotentiometer(POT_PIN), 0, 1023, 1, LEVELS);
        if (level != tmp) {
            printOnConsole("Level: " + String(level));
        }
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
    if (millis() - reference <= t1) {
        if (millis() - referenceBlinkLed > ballSpeed) {
            turnOffLed(leds[ballPosition]);
            updateDirection();
            ballPosition += ballDirection;
            referenceBlinkLed = millis();
        } else {
            turnOnLed(leds[ballPosition]);
        }
    } else {
        changeGameStatus(PLAY);
        enableInterruptsOnButtons();
    }
}

void gamePlay() {
    long btnPressed = getButtonPressed();
    if (millis() - reference > t2 || (btnPressed != -1 && btnPressed != ballPosition)) {
        turnOffLeds();
        printOnConsole("Game Over. Final Score: " + String(score));
        changeGameStatus(OVER);
        disableInterruptsOnButtons();
    } else if (btnPressed == ballPosition) {
        updateGameParameters();
        printOnConsole("New point! Score:" + String(score));
        turnOffLeds();
        changeGameStatus(BLINK);
        disableInterruptsOnButtons();
    }
}

void sleep() {
    turnOffLeds();
    printOnConsole("Sleep!");
    deepSleepSystem();
    gameStatus = SETUP;
}

void gameOver() {
    if (millis() - reference > TIMEOUT) {
        gameStatus = SETUP;
        enableInterruptsOnButtons();
    }
}
