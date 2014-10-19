#pragma once

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"
#include "Buffer.h"
#include <Windows.h>
#include <vector>
#include <time.h>
#include <map>
#include <string>

const int TWO_K = 0, THREE_K = 1, FOUR_K = 5, FIVE_K = 25, STRAIGHT = 10, IO_R_FLUSH = 85, UO_R_FLUSH = 4;
const bool DEBUG = false;

const byte BORDER_COLOUR = F_PURPLE + B_GREY;

class PokerGame {
private:
    Reel reels[5];
    bool slotsRunning = true;
    Buffer buffer;
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
    void print(string, byte);
    void print(char, byte);
    void print(char *, byte);
    void printRainbow(std::string);
};
