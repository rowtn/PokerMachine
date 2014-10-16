#pragma once

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"
#include <Windows.h>
#include <vector>
#include <time.h>
#include <map>
#include <string>

const int TWO_K = 0, THREE_K = 1, FOUR_K = 5, FIVE_K = 25, STRAIGHT = 10, IO_R_FLUSH = 85, UO_R_FLUSH = 4;

enum Colour {
    BLACK, DARK_BLUE, DARK_GREEN, DARK_AQUA, DARK_RED, PURPLE, GOLD, LIGHT_GREY, DARK_GREY, BLUE, LIGHT_GREEN, LIGHT_AQUA, RED, PINK, YELLOW
};

const Colour BORDER_COLOUR = PURPLE;

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
    //resets the reels for another round
    void resetReels(void);
    //calculations to be done every frame
    void gameloop(void);
    //Checks for wins, returns credits won
    int checkWins(void);
    //print text with colour.
    void print(char *, Colour);
    void print(char, Colour);
};
