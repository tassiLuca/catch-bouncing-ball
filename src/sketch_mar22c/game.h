#ifndef __GAME_H__
#define __GAME_H__

/**
 * 
 */
typedef enum {
    SETUP,
    READY,
    BLINK,
    PLAY,
    OVER,
    SLEEP
} GameStatus;

typedef enum {
    RIGHT = 1,
    LEFT = -1
} BallDirection;

/**
 * Init the game. Print a welcome message.
 */
void welcome();

/**
 * The game is ready to play.
 */
void gameReady();


void gameBlink();

/**
 * The game starts!
 */
void gamePlay();

/** 
 * Go in sleep.
 */
void sleep();

void gameOver();

#endif
