#include <string.h>
#include "Arduino.h"
#include "game.h"
#include "input.h"

GameStatus gameStatus;

static buttons 
static String welcomeMsg = 
    "************************************************ \n"
    "Welcome to the Catch the Bouncing Led Ball Game. \n"
    "Press Key T1 to Start. \n"
    "************************************************ \n";

void welcome() {
    printOnConsole(welcomeMsg);
    gameStatus = READY;
}

void gameReady() {
    
}
