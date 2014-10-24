#pragma once
#include "Buffer.h"

enum Direction {
    N, NE, E, SE, S, SW, W, NW, STOP
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
    Paddle paddle = { 22, 30 };
    //I cache the amount of blocks left on the screen so I do not have to iterate through the whole array to get the count
    int blocksLeft = 150, printOffset = 4;
    IntPair ballLocation = { 25, 29 };
    Direction ballDir = STOP;
    int paddleX, paddleY, lives = 3;
    int blocks[3][50];
    const char ball = char(178);
    bool gameRunning = true;
    bool won = false;
    bool quit = false;
    //const std::string paddleAvatar = { char(205), char(205), char(205) };
public:
    BreakOut();
    void gameloop();
    Direction getBounceDirection(Direction);
};
