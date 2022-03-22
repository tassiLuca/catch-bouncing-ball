#ifndef __GAME_H__
#define __GAME_H__

typedef enum {
    SETUP,
    READY,
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

/** 
 * Go in sleep.
 */
void sleep();

#endif
