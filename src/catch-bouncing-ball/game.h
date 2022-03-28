#ifndef __GAME_H__
#define __GAME_H__

/**
 * An enum describing the states of the game.
 */
typedef enum {
    SETUP,
    READY,
    BLINK,
    PLAY,
    OVER,
    SLEEP
} GameStatus;

/** 
 * A global variable which describes the current state of the game .
 */
extern GameStatus gameStatus;

/**
 * Init the game. Prints a welcome message.
 */
void welcome();

/**
 * Waits for the user pression of the first button in order to start
 * the game. If, after a reasonable amount of time nothing happens, 
 * the system goes in deep sleeping.
 */
void gameReady();

/** 
 * The ball moves repeatedly back and forth for a fixed amount of time.
 */
void gameBlink();

/**
 * Implements the game logic.
 */
void gamePlay();

/** 
 * Put the system in deep sleeping.
 */
void sleep();

/**
 * The play is finished. Prints a message with the score.
 */
void gameOver();

#endif
