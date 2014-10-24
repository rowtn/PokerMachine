#pragma once
#include "Buffer.h"

enum Direction {
    N, NE, E, SE, S, SW, W, NW
};

struct Paddle {
    int x, y;
    Paddle(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class BreakOut {
private:
    Buffer buffer = { 60, 60, B_GREY };
    Paddle paddle = { 13, 30 };
    //I cache the amount of blocks left on the screen so I do not have to iterate through the whole array to get the count
    int blocksLeft = 30, printOffset = 14;
    IntPair ballLocation = { 15, 15 };
    Direction ballDir = S;
    int paddleX, paddleY;
    int blocks[3][15];
    const char ball = char(178);
    bool gameRunning = true;
    //const std::string paddleAvatar = { char(205), char(205), char(205) };
public:
    BreakOut();
    void gameloop();
};
