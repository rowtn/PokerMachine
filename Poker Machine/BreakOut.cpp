#include "stdafx.h"
#include "BreakOut.h"
#include <string>

BreakOut::BreakOut() {
    //ballLocation.second = 15;
    buffer.clear();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 30; j++) {
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
    byte colours[3] = { F_LIGHT_RED, F_LIGHT_AQUA, F_LIGHT_GREEN };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 30; j++) {
            if (blocks[i][j] > 0) {
                buffer.writeAt(char(178), printOffset + 1 + j, 3 + i * 2, colours[blocks[i][j] - 1], B_GREY);
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
    case SW:
        ballLocation.second++;
        ballLocation.first++;
        break;
    case SE:
        ballLocation.second++;
        ballLocation.first--;
        break;
    }
    if (ballLocation.first == 1 || ballLocation.first == 30) {
        switch (ballDir) {
        case NE:
            ballDir = NW;
            break;
        case NW:
            ballDir = NE;
            break;
        case SE:
            ballDir = SW;
            break;
        case SW:
            ballDir = SE;
            break;
        }
    }
    if (ballLocation.second == 3) {
        switch (ballDir) {
        case N:
            ballDir = S;
            break;
        case NE:
            ballDir = SW;
            break;
        case NW:
            ballDir = SE;
            break;
        default:
            ballDir = S; //failsafe
            break;
        }
    }
    if (ballLocation.second > paddle.y + 2) {
        gameRunning = false;
    }
    if (ballLocation.second == paddle.y - 1) {
        //TODO: Explicit checks
        if (ballLocation.first > paddle.x + 2 && ballLocation.first < paddle.x + 5) {
            ballDir = N;
        } 
        if (ballLocation.first <= paddle.x + 2) {
            ballDir = NW;
        } 
        if (ballLocation.first >= paddle.x + 4) {
            ballDir = NE;
        }
    }
    buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_LIGHT_AQUA, B_GREY);

    buffer.print();
    Sleep(40);
}
