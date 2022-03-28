#include "setup.h"
#include "boundary.h"
#include "game.h"

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
