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
    while (gameRunning) gameloop();
    if (quit) return; //if player elected to quit, go back to main menu
    buffer.clear();
    buffer.skipLine(5);
    /* display info */
    if (won) {
        buffer.writeCentered("Congrats! You won!", F_BLACK, B_GREY);
    } else {
        buffer.writeCentered("Oh no, you lost. Better luck next time.", F_BLACK, B_GREY);
    }
    buffer.skipLine(2);
    buffer.writeCentered("Press SPACE to return to the main menu.", F_BLACK, B_GREY);
    buffer.print();
    system("pause>nul");
keyListen:
    if (!GetAsyncKeyState(VK_SPACE)) {
        goto keyListen;
    }
}

void BreakOut::gameloop() {
    /* clear buffer */
    buffer.clear();
    /* write walls */
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            if (i == 0 || i == 31 || j == 0 || j == 31) buffer.writeAt(char(219), printOffset + i, 2 + j, F_BRIGHT_WHITE, B_GREY);
        }
    }

    /* show lives left */
    buffer.setCursorPosition(printOffset, 34); //moves cursor to suitable position
    buffer.write("Lives: ", F_BLACK, B_GREY);
    for (int i = 1; i <= lives; i++) {
        // { char(3) } turns the char into a string using string's contructor for char *
        buffer.write({ char(3) }, F_LIGHT_RED, B_GREY); //writes lives as hearts onto buffer
    }
    buffer.setCursorPosition(printOffset, 35);
    buffer.write("Blocks left: ", F_BLACK, B_GREY);
    //to_string is required here. if it were not there, it will display the ASCII character that corresponds with the value of blocksLeft
    buffer.write(std::to_string(blocksLeft), F_DARK_AQUA, B_GREY);
    /* print bricks */
    byte colours[3] = { F_LIGHT_RED, F_LIGHT_AQUA, F_LIGHT_GREEN };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 30; j++) {
            //if the brick at this index is still 'alive'
            //each time a block is hit, this number is decremented, and once it is 0 or lower, it is 'dead' and should no longer be displayed
            if (blocks[i][j] > 0) {
                //get's all bricks and writes the to buffer with appropriate colour and position
                buffer.writeAt(char(178), printOffset + 1 + j, 3 + i * 2, colours[blocks[i][j] - 1], B_GREY);
            }
        }
    }

    /* move ball */
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
    /* Ball hitting paddle */
    if (ballLocation.second == paddle.y - 1) {
        if (ballLocation.first == paddle.x + 3) {
            ballDir = N;
        } else if (ballLocation.first <= paddle.x + 2 && ballLocation.first >= paddle.x) {
            ballDir = NW;
        } else if (ballLocation.first >= paddle.x + 4 && ballLocation.first <= paddle.x + 6) {
            ballDir = NE;
        }
    }
    /* move paddle */
    if (GetAsyncKeyState(VK_LEFT)) {
        if (paddle.x > 0) paddle.x--;
    } else if (GetAsyncKeyState(VK_RIGHT)) {
        if (paddle.x < 25) paddle.x++;
    }

    /* print paddle */
    for (int i = 1; i <= 5; i++) {
        //paddle is 5 chars long, for loop simplifies printing
        buffer.writeAt(char(219), paddle.x + printOffset + i, paddle.y, F_PURPLE, B_GREY);
    }

    /* wall bounce */
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
    /* loss. reset game and subtract life */
    if (ballLocation.second > paddle.y + 1) {
        buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_LIGHT_RED, B_GREY); //change ball colour to red
        buffer.writeAt(char(0), printOffset + 6 + lives, 34, F_GREY, B_GREY); //removes last heart from buffer
        lives--; //decrements lives counter
        buffer.print(); //reprint onto screen with above modifications
        //start reset
        ballDir = S;
        paddle = { 12, 30 };
        ballLocation = { 15, 15 };
        //end reset

        //cursor to suitable position
        buffer.setCursorPosition(printOffset, 37);
        if (lives > 0) {
            buffer.write("Would you like to play again?", F_LIGHT_GREEN, B_GREY);
            buffer.setCursorPosition(printOffset, 38);
            buffer.write("Press ENTER to continue or ESC", F_LIGHT_GREEN, B_GREY);
            buffer.setCursorPosition(printOffset, 39);
            buffer.write("to go back to the main menu.", F_LIGHT_GREEN, B_GREY);
            buffer.print();
            system("pause>nul");
        keyListen:
            if (GetAsyncKeyState(VK_ESCAPE)) { //quit to menu
                gameRunning = false;
                quit = true;
            //if any key other than enter, return to keyListen
            //the next if will only pass if ENTER is pressed. if ENTER is pressed, it will continue the game
            } else if (!GetAsyncKeyState(VK_RETURN)) {
                goto keyListen;
            }
        //if out of lives
        } else {
            gameRunning = false;
            won = false;
        }
    }

    //top row
    if (ballLocation.second == 3 && blocks[0][ballLocation.first - 1] > 0) {
        blocks[0][ballLocation.first - 1]--; //decrements value at required position
        blocks[0][ballLocation.first]--;
        blocksLeft--;
        ballDir = getBounceDirection(ballDir);
    //middle row
    } else if (ballLocation.second == 5 && blocks[1][ballLocation.first - 1] > 0) {
        blocks[1][ballLocation.first - 1]--;
        blocks[1][ballLocation.first]--;
        ballDir = getBounceDirection(ballDir);
        blocksLeft--;
    //bottom row
    } else if (ballLocation.second == 7 && blocks[2][ballLocation.first - 1] > 0) {
        blocks[2][ballLocation.first - 1]--;
        blocks[2][ballLocation.first]--;
        ballDir = getBounceDirection(ballDir);
        blocksLeft--;
    //roof
    } else if (ballLocation.second == 3) {
        ballDir = getBounceDirection(ballDir);
        blocksLeft--;
    }
    //write ball onto screen at suitable position
    buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_YELLOW, B_GREY);
    buffer.print();
    //if all blocks have been destroyed
    if (blocksLeft == 0) {
        won = true;
        gameRunning = false;
    }
    clock_t t = clock();
    while (clock() - t < 33.33) { /* 33.33ms delay */ }
}

//returns the relative bounce direction for all directions
Direction BreakOut::getBounceDirection(Direction dir) {
    switch (dir) {
    case N:
        return S;
    case NE:
        return SW;
    case NW:
        return SE;
    case S:
        return N;
    case SE:
        return NW;
    case SW:
        return NE;
    }
}
