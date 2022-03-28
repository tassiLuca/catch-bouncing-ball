#include "Arduino.h"
#include <math.h>
#include "setup.h"
#include "game.h"
#include "boundary.h"

/** The function which determines the speed reduce factor. */
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
 * Re-maps a number from one range to another, like map() does but returning a floating value.
 */
static float mapfloat(long x, long fromLow, long fromHigh, float toLow, float toHigh) {
  return (float)(x - fromLow) * (toHigh - toLow) / (float)(fromHigh - fromLow) + toLow;
}

/**
 * Change the game status, resets timers variables and, on OVER and BLINK
 * states, disables buttons interrupts: this is necessary to avoid in the
 * following checks are considered not useful pressions.
 */
static void changeGameStatus(GameStatus newStatus) {
    static bool interruptsDisabled = false;
    gameStatus = newStatus;
    reference = millis();
    referenceBlinkLed = millis();
    if (newStatus == BLINK || newStatus == OVER) {
        disableInterruptsOnButtons();
        interruptsDisabled = true;
    } else if (interruptsDisabled) {
        enableInterruptsOnButtons();
        interruptsDisabled = false;
    }
}

/**
 * Updates the game parameters according to the current game status: if it
 * is "PLAY" the parameters are updated incrementally, otherwise are reset.
 */
static void updateGameParameters() {
    ballPosition = STARTING_BALL_POS;
    ballDirection = RIGHT;
    // [NOTE] t1 is measured in ms.
    t1 = ((random() % RAND_MAX_TIME) + RAND_MIN_TIME) * 1000;
    if (gameStatus == PLAY) { // update the parameters
        t2 = max(t2 / reduceFactor, 0);
        ballSpeed = max(ballSpeed / REDUCE_FACTOR(level), MIN_BALL_SPEED);
        score += 1;
    } else { // reset the paramaters
        t2 = DEFAULT_T2;
        ballSpeed = DEFAULT_BALL_SPEED;
        score = 0;
        reduceFactor = mapfloat(level, 1, LEVELS, MIN_REDUCE_FACTOR, MAX_REDUCE_FACTOR);
    }
#ifdef DBG
    printOnConsole("Ball speed " + String(ballSpeed));
    printOnConsole("You have " + String(t2) + "ms to push the correct button");
#endif
}

void welcome() {
    turnOffLeds();
    printOnConsole(WELCOME_MSG);
    changeGameStatus(READY);
}

void gameReady() {
    if (millis() - reference > TIMEOUT) {
        changeGameStatus(SLEEP);
    } else if (getButtonPressed() == 0) {
        turnOffLeds();
        printOnConsole("Go!");
        changeGameStatus(BLINK);
        updateGameParameters();
    } else {
        fadeLed(LS_PIN);
        int tmp = level;
        // [NOTE] Please note the fromHigh and toHigh are augmented by 1 unit
        //        to maps [0, 1023] onto [1, 8] evenly distributed, 
        //        avoiding only 1023 is mapped into LEVELS value!
        // See [https://create.arduino.cc/projecthub/dhorton668/a-better-mapping-with-map-dd66f5?ref=part&ref_id=8233&offset=5]
        level = map(readPotentiometer(POT_PIN), 0, 1024, 1, LEVELS + 1);
        if (level != tmp) {
            printLevel(level, LEVELS);
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
    if (millis() - reference <= (unsigned) t1) {
        if (millis() - referenceBlinkLed > (unsigned) ballSpeed) {
            turnOffLed(leds[ballPosition]);
            updateDirection();
            ballPosition += ballDirection;
            referenceBlinkLed = millis();
        } else {
            turnOnLed(leds[ballPosition]);
        }
    } else {
        changeGameStatus(PLAY);
    }
}

void gamePlay() {
    long btnPressed = getButtonPressed();
    if (millis() - reference > (unsigned) t2 || (btnPressed != -1 && btnPressed != ballPosition)) {
        turnOffLeds();
        printOnConsole(">>> Game Over. Final Score: " + String(score));
        changeGameStatus(OVER);
    } else if (btnPressed == ballPosition) {
        updateGameParameters();
        printOnConsole("New point! Score:" + String(score));
        turnOffLeds();
        changeGameStatus(BLINK);
    }
}

void sleep() {
    turnOffLeds();
    printOnConsole("Sleep!");
    deepSleepSystem();
    changeGameStatus(SETUP);
}

void gameOver() {
    if (millis() - reference > TIMEOUT) {
        changeGameStatus(SETUP);
    }
}
