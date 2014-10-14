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
    //display cards on screen
    void display(void);
    //initialize slot machine
    void init(void);
    //calculations to be done every frame
    void gameloop(void);
    //Checks for wins, returns credits won
    int checkWins(void);
};