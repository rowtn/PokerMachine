#pragma once
#include "Buffer.h"

struct IntPair {
    int x, y;
    IntPair(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class BreakOut {
private:
    IntPair boardSize = { 50, 40 };
    Buffer buffer = { 60, 60, F_GREY };
public:
    BreakOut();
};

class BreakOutPlayer {
private:
    IntPair position;

};

typedef BreakOutPlayer Player; //Player is now an alias of BreakOutPlayer
