#include "setup.h"
#include "input.h"
#include "game.h"

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
        case PLAY:
            gamePlay();
            break;
        case SLEEP:
            sleep();
            break;
        default:
            break;
    }
}
