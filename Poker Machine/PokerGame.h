#pragma once

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"
#include <Windows.h>
#include <vector>
#include <time.h>
#include <map>

class PokerGame {
private:
    Reel reels[5];
    bool slotsRunning = true;
public:
    PokerGame();
    /* Method prototypes */
    void display(void);     //display cards on screen
    void init(void);        //initialize slot machine
    void gameloop(void);    //calculations to be done every frame
    int checkWins(void);    //Checks for wins, returns credits won
};
