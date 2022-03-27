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
