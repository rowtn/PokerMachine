#include "stdafx.h"
#include "PokerGame.h"
#include "Buffer.h"
#include <Windows.h>

using namespace std;

Buffer buffer(30, 30);

int _tmain(int argc, _TCHAR* argv[]) {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
    //PokerGame();
    //buffer.write("Bye, World!");
    //buffer.print();
    buffer.write("test\ntest");
    buffer.print();
    cin.get();
    buffer.clear();
    buffer.print();
    cin.get();
    buffer.write("foo", F_DARK_AQUA);
    buffer.write("\nbar", F_DARK_RED);
    buffer.skipLine(4);
    buffer.write("test", F_DARK_BLUE, B_LIGHT_RED);
    buffer.print();
    cin.get();
    return 0;
}
