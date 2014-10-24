#include "stdafx.h"
#include "BreakOut.h"
#include <string>

BreakOut::BreakOut() {
    //ballLocation.second = 15;
    paddle.x = 12;
    buffer.clear();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 30; j++) {
            blocks[i][j] = 3 - i;
        }
    }
    while (lives > 0) gameloop();
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
    buffer.setCursorPosition(printOffset, 34);
    buffer.write("Lives: ", F_BLACK, B_GREY);
    for (int i = 1; i <= lives; i++) {
        buffer.write({ char(3) }, F_LIGHT_RED, B_GREY);
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

    /* loss. reset game and subtract life */
    if (ballLocation.second > paddle.y + 1) {
        buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_LIGHT_RED, B_GREY);
        buffer.writeAt(char(0), printOffset + 6 + lives, 34, F_GREY, B_GREY);
        buffer.print();
        lives--;
        ballDir = S;
        paddle = { 12, 30 };
        ballLocation = { 15, 15 };
        system("pause>nul");
    }
    if (ballLocation.second == paddle.y - 1) {
        //TODO: Explicit checks
        if (ballLocation.first > paddle.x + 2 && ballLocation.first < paddle.x + 5) {
            ballDir = N;
        } 
        if (ballLocation.first <= paddle.x + 2 && ballLocation.first >= paddle.x) {
            ballDir = NW;
        } 
        if (ballLocation.first >= paddle.x + 4 && ballLocation.first <= paddle.x + 5) {
            ballDir = NE;
        }
    }
    buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_LIGHT_AQUA, B_GREY);

    buffer.print();
    Sleep(40);
}
