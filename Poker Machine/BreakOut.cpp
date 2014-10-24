#include "stdafx.h"
#include "BreakOut.h"
#include <string>

BreakOut::BreakOut() {
    //ballLocation.second = 15;
    buffer.clear();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 15; j++) {
            blocks[i][j] = 3 - i;
        }
    }
    while (gameRunning) gameloop();
    buffer.clear();
    buffer.skipLine(5);
    buffer.writeCentered("u lost m9", F_BLACK, B_GREY);
    buffer.print();
    system("pause>nul");
}

void BreakOut::gameloop() {
    buffer.clear();
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            if (i == 0 || i == 31 || j == 0 || j == 31) buffer.writeAt(char(219), printOffset + i, 2 + j, F_BRIGHT_WHITE, B_GREY);
        }
    }
    byte colours[3] = { F_LIGHT_RED, F_DARK_AQUA, F_DARK_GREEN };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 15; j++) {
            if (blocks[i][j] > 0) {
                buffer.writeAt(char(201), printOffset + 1 + j * 2, 3 + i * 2, colours[blocks[i][j] - 1], B_GREY);
                buffer.writeAt(char(187), printOffset + 2 + j * 2, 3 + i * 2, colours[blocks[i][j] - 1], B_GREY);
            }
        }
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        if (paddle.x > 0) paddle.x--;
    } else if (GetAsyncKeyState(VK_RIGHT)) {
        if (paddle.x < 25) paddle.x++;
    }
    for (int i = 1; i <= 5; i++) {
        buffer.writeAt(char(219), paddle.x + printOffset + i, paddle.y, F_PURPLE, B_GREY);
    }
    switch (ballDir) {
    case S:
        ballLocation.second++;
        break;
    case N:
        ballLocation.second--;
        break;
    case NE:
        ballLocation.second--;
        ballLocation.first++;
        break;
    case NW:
        ballLocation.second--;
        ballLocation.first--;
        break;
    }
    if (ballLocation.second < 5) {
        ballDir = S;
    }
    if (ballLocation.second > paddle.y + 2) {
        gameRunning = false;
    }
    if (ballLocation.second == paddle.y - 1) {
        //TODO: Explicit checks
        if (ballLocation.first > paddle.x + 1 && ballLocation.first < paddle.x + 4) {
            ballDir = N;
        } 
        if (ballLocation.first == paddle.x + 1) {
            ballDir = NW;
        } 
        if (ballLocation.first == paddle.x + 5) {
            ballDir = NE;
        }
    }
    buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_LIGHT_AQUA, B_GREY);

    buffer.print();
    Sleep(100);
}
