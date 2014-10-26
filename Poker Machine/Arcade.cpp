﻿#include "stdafx.h"
#include "PokerGame.h"
#include "Buffer.h"
#include "BreakOut.h"
#include <Windows.h>
#include <fstream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {
    /* Resize window */
    system("mode 60,60");
    SMALL_RECT WinRect = { 0, 0, 60, 60 };
    SMALL_RECT* WinSize = &WinRect;
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, WinSize);

    /* init */
    system("title Arcade - Menu");
    Buffer buffer(60, 60, B_GREY); //instantiate buffer with size 60, 60, and default colours grey background and black foreground
    bool exitRequested = false;
    /* Disable caret and input echo (makes whatever is being typed in invisible) 
    http://www.cplusplus.com/forum/beginner/118447/#msg645990 input echo
    */
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

    /* Start menu */
    int selection = 0;
    string menuOptions[3] = { "Poker Machine", "BreakOut!", "Exit" };
    byte logoF = F_LIGHT_GREEN, logoBg = B_GREY;
    int ignoreInput = 0;
    /* Use of R"(<string>)" allows me to write special chars without escaping e.g. '\' instead of '\\' */
    string asciiLogo[7] = { 
        R"(  ____  ____      __   ____  ___      ___   _   )",
        R"( /    ||    \    /  ] /    ||   \    /  _] | |  )",
        R"(|  o  ||  D  )  /  / |  o  ||    \  /  [_  | |  )",
        R"(|     ||    /  /  /  |     ||  D  ||    _] | |  )",
        R"(|  _  ||    \ /   \_ |  _  ||     ||   [_  | |  )",
        R"(|  |  ||  .  \\     ||  |  ||     ||     | |_|  )",
        R"(|__|__||__|\_| \____||__|__||_____||_____|  o   )"
    };
    int scroll = 0;
    while (!exitRequested) {
        static clock_t t = clock();
        bool scroll = false;
        if (clock() - t > 50) {
            t = clock();
            scroll = true;
        }
        for (std::string &text : asciiLogo) {
            if (scroll) text = text.substr(1, text.length()) + text.substr(0, 1);
            buffer.writeCentered(text.substr(0, text.length() <= 58 ? text.length() : 58), logoF + logoBg);
        }
        buffer.skipLine(2);
        for (int i = 0; i < 3; i++) {
            buffer.skipLine(1);
            //Highlight selection and flash
            if (i == selection) {
                //using clock() % 500 > 250 in the ternary operations below ensure that the selection will flash every 250ms
                buffer.writeCentered((clock() % 500 > 250 ? "> " : "- ") + menuOptions[i] + (clock() % 500 > 250 ? " <" : " -"), F_LIGHT_AQUA + B_GREY);
            } else {
                buffer.writeCentered(menuOptions[i], F_BLACK + B_GREY);
            }
            buffer.skipLine(1);
        }
        ignoreInput++;
        //This will ignore the input most of the time. Without this, the selection would move too quickly,
        //as the other code runs extremely fast (Using double buffering as opposed to std::cout is to blame for this)
        if (ignoreInput > 50) { 
            if (GetAsyncKeyState(VK_UP)) {
                selection = selection == 0 ? 2 : --selection;
                PlaySound(TEXT("menu-move.wav"), NULL, SND_ASYNC);
            } else if (GetAsyncKeyState(VK_DOWN)) {
                selection = (selection + 1) % 3; //always value between 0 - 2
                PlaySound(TEXT("menu-move.wav"), NULL, SND_ASYNC);
            } else if (GetAsyncKeyState(VK_ESCAPE)) {
                exitRequested = true;
                PlaySound(TEXT("menu-move.wav"), NULL, SND_ASYNC);
            } else if (GetAsyncKeyState(VK_RETURN)) {
                PlaySound(TEXT("menu-select.wav"), NULL, SND_ASYNC);
                switch (selection) {
                case 0:
                    PokerGame();
                    break;
                case 1:
                    BreakOut();
                    break;
                case 2:exitRequested = true;
                    break;
                default:
                    break;
                }
            }
            ignoreInput = 0;
        }
        buffer.print();
        buffer.clear();
    }
    return 0;
}
