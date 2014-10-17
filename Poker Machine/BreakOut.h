#pragma once
#include "Buffer.h"
#include "BreakOutUtils.h"

class BreakOut {

private:
    IntPair boardSize = { 50, 40 };
    Buffer buffer = { 60, 60, F_GREY };
public:
    BreakOut();
};
