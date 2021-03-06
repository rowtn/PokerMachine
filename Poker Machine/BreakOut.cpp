#include "stdafx.h"
#include "BreakOut.h"
#include "AsyncSound.h"
#include <string>
#include <thread>

BreakOut::BreakOut() {
    system("title Arcade! - BreakOut!");
    buffer.clear();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 50; j++) {
            blocks[i][j] = 3 - i;
        }
    }
    while (gameRunning) gameloop();
    if (quit) return; //if player elected to quit, go back to main menu
    buffer.clear();
    buffer.skipLine(5);
    int score = (150 - blocksLeft) * 100; //final score
    int display = 0; //number to display
    //this block will display the score in a fancy way 
    while (display != score) {
        //increment display by 10 until it is equal to the final score
        buffer.writeCentered("Score: " + std::to_string(display += 10), F_YELLOW + B_GREY);
        std::thread(Beep, display, 20).detach();
        buffer.print();
        buffer.skipLine(-1); //move line back up to overwrite Score display line
    }
    buffer.skipLine(5);
    /* display info */
    if (won) {
        buffer.writeCentered("Congrats! You won!", F_BLACK + B_GREY);
    } else {
        buffer.writeCentered("Oh no, you lost. Better luck next time.", F_BLACK + B_GREY);
    }
    buffer.skipLine(2);
    buffer.writeCentered("Press SPACE to return to the main menu.", F_BLACK + B_GREY);
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
    for (int i = 0; i < 52; i++) {
        for (int j = 0; j < 32; j++) {
            if (i == 0 || i == 51 || j == 0 || j == 31) buffer.writeAt(char(219), printOffset + i, 2 + j, F_BRIGHT_WHITE + B_GREY);
        }
    }

    if (ballDir == STOP) {
        buffer.setCursorPosition(0, 15);
        buffer.writeCentered("Press SPACE to begin!", F_BLACK + B_GREY);
    }

    /* show lives left */
    buffer.setCursorPosition(printOffset + 15, 34); //moves cursor to suitable position
    buffer.write("Lives: ", F_BLACK + B_GREY);
    for (int i = 1; i <= 3; i++) {
        // { char(3) } turns the char into a string using string's contructor for char *
        buffer.write({ char(3) }, (i <= lives ? F_LIGHT_RED : F_BLACK) + B_GREY); //writes lives as hearts onto buffer. lives lost are displayed as black
    }
    buffer.setCursorPosition(printOffset + 15, 35);
    buffer.write("Blocks left: ", F_BLACK + B_GREY);
    //to_string is required here. if it were not there, it will display the ASCII character that corresponds with the value of blocksLeft
    buffer.write(std::to_string(blocksLeft), F_DARK_AQUA + B_GREY);
    /* print bricks */
    byte colours[3] = { F_LIGHT_RED, F_LIGHT_AQUA, F_LIGHT_GREEN };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 50; j++) {
            //if the brick at this index is still 'alive'
            //each time a block is hit, this number is decremented, and once it is 0 or lower, it is 'dead' and should no longer be displayed
            if (blocks[i][j] > 0) {
                //get's all bricks and writes the to buffer with appropriate colour and position
                buffer.writeAt(char(178), printOffset + 1 + j, 3 + i * 2, colours[blocks[i][j] - 1] + B_GREY);
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
    if (ballLocation.second == paddle.y - 1 && ballDir != STOP) {
        if (ballLocation.first >= paddle.x && ballLocation.first <= paddle.x + paddle.length) {
            ballDir = getBounceDirection(ballDir);
            if (paddle.x + 1 == ballLocation.first) ballDir = NW;
            else if (paddle.x + paddle.length == ballLocation.first) ballDir = NE;
        }
    }
    /* move paddle */
    if (GetAsyncKeyState(VK_LEFT)) {
        if (paddle.x > 0) {
            paddle.x -= 2;
            if (ballDir == STOP) ballLocation.first -= 2;
        }
    } else if (GetAsyncKeyState(VK_RIGHT)) {
        if (paddle.x < 44) {
            paddle.x += 2;
            if (ballDir == STOP) ballLocation.first += 2;
        }
    }

    /* print paddle */
    for (int i = 1; i <= paddle.length; i++) {
        buffer.writeAt(char(219), paddle.x + printOffset + i, paddle.y, F_PURPLE + B_GREY);
    }

    /* wall bounce */
    if (ballLocation.first == 1 || ballLocation.first == 50) {
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
        //sound source: http://www.soundjay.com/misc/sounds/fail-trombone-01.mp3
        playSoundAsync(L"poker-lose.wav");
        buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_LIGHT_RED + B_GREY); //change ball colour to red
        buffer.writeAt(char(3), printOffset + 21 + lives, 34, F_BLACK + B_GREY); //removes last heart from buffer
        lives--; //decrements lives counter
        buffer.print(); //reprint onto screen with above modifications
        //start reset
        ballDir = STOP;
        paddle = { 22, 30, 5 };
        ballLocation = { 25, 29 };
        //end reset

        //cursor to suitable position
        buffer.setCursorPosition(printOffset, 37);
        if (lives > 0) {
            buffer.write("Would you like to play again?", F_LIGHT_GREEN + B_GREY);
            buffer.setCursorPosition(printOffset, 38);
            buffer.write("Press ENTER to continue or ESC", F_LIGHT_GREEN + B_GREY);
            buffer.setCursorPosition(printOffset, 39);
            buffer.write("to go back to the main menu.", F_LIGHT_GREEN + B_GREY);
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
            buffer.setCursorPosition(0, 15);
            buffer.writeCentered("You lost! Press any key to continue.", F_BLACK + B_GREY);
            buffer.print();
            //a delay before pausing ensures the player doesn't accidently press a button
            //without the chance of reading 'You lost!'
            clock_t t = clock();
            while (clock() - t < 1000) { /* 1 second delay */ }
            system("pause>nul");
            gameRunning = false;
            won = false;
        }
    }

    //top row
    if (ballLocation.second == 3 && blocks[0][ballLocation.first - 1] > 0) {
        std::thread(Beep, blocks[0][ballLocation.first - 1] * 100 + 500, 100).detach();
        blocks[0][ballLocation.first - 1]--; //decrements value at required position
        blocks[0][ballLocation.first]--;
        blocksLeft--;
        ballDir = getBounceDirection(ballDir);
        //middle row
    } else if (ballLocation.second == 5 && blocks[1][ballLocation.first - 1] > 0) {
        std::thread(Beep, blocks[1][ballLocation.first - 1] * 100 + 500, 100).detach();
        blocks[1][ballLocation.first - 1]--;
        blocks[1][ballLocation.first]--;
        ballDir = getBounceDirection(ballDir);
        blocksLeft--;
        //bottom row
    } else if (ballLocation.second == 7 && blocks[2][ballLocation.first - 1] > 0) {
        std::thread(Beep, blocks[2][ballLocation.first - 1] * 100 + 500, 100).detach();
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
    buffer.writeAt(ball, ballLocation.first + printOffset, ballLocation.second, F_YELLOW + B_GREY);
    buffer.print();
    //if all blocks have been destroyed
    if (blocksLeft == 0) {
        //sound source: http://www.freesfx.co.uk/download/?type=mp3&id=3928
        playSoundAsync(L"poker-win.wav");
        won = true;
        gameRunning = false;
    }
    //this code is at the bottom so that the screen prints out. This ensures that the spacebar can only be pressed at the start of the game
    //to start it. the ballDir will only ever be STOP at the start of each life
    if (ballDir == STOP && GetAsyncKeyState(VK_SPACE)) ballDir = N;
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
    case STOP:
        return STOP;
    }
}
