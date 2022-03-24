#include "setup.h"
#include "input.h"
#include "game.h"

// Uncomment to print useful logs in Serial
// #define DBG

extern GameStatus gameStatus;

void setup() {
    initSystem();
    gameStatus = SETUP;
}

void loop() {
    switch(gameStatus){
        case SETUP:
            welcome();
            break;
        case READY:
            gameReady();
            break;
        case BLINK:
            gameBlink();
            break;
        case PLAY:
            gamePlay();
            break;
        case SLEEP:
            sleep();
            break;
        case OVER:
            gameOver();
            break;
        default:
            break;
    }
}
