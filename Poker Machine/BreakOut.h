#pragma once
#include "Buffer.h"

enum Direction {
    N, NE, E, SE, S, SW, W, NW
};

class BreakOut {
private:
    Buffer buffer = { 60, 60, B_GREY };
    int blocks[3][15] = {
            { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
            { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };
    //I cache the amount of blocks left on the screen so I do not have to iterate through the whole array to get the count
    int blocksLeft = 30;
    IntPair ballLocation;
    int paddleX, paddleY;
    const char paddleAvatar[3] = { char(205), char(205), char(205) };
public:
    BreakOut();
    void gameloop();
};
