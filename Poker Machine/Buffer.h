#pragma once

#include "Windows.h"

const byte F_BLACK = 0x0000, F_DARK_BLUE = 0x0001, F_DARK_GREEN = 0x0002, F_DARK_AQUA = 0x0003, F_DARK_RED = 0x0004, F_PURPLE = 0x0005, F_GOLD = 0x0006, F_WHITE = 0x0007,
F_LIGHT_GREEN = 0x000a, F_LIGHT_AQUA = 0x000b, F_LIGHT_RED = 0x000c, F_PINK = 0x000d, F_YELLOW = 0x000e, F_BRIGHT_WHITE = 0x000f,
//B_DARK_BLUE = 0x0010, B_DARK_GREEN = 0x0020, B_DARK_AQUA = 0x0030, B_DARK_RED = 0x0040;
B_BLACK = 0x000, B_DARK_BLUE = 0x010, B_DARK_GREEN = 0x020, B_DARK_AQUA = 0x030, B_DARK_RED = 0x040, B_PURPLE = 0x050, B_GOLD = 0x060, B_WHITE = 0x07,
B_LIGHT_GREEN = 0x0a0, B_LIGHT_AQUA = 0x0b0, B_LIGHT_RED = 0x0c0, B_PINK = 0x0d0, B_YELLOW = 0x0e0, B_BRIGHT_WHITE = 0x0f0;

class Buffer {
private:
    HANDLE handle;

    COORD size;
    COORD coord;
    SMALL_RECT region;

    CHAR_INFO buffer[1000];

    int column = 0, row = 0;
public:
    Buffer();
    Buffer(int, int);
    void print(void); //done
    void write(std::string); //done
    void writeLine(std::string); //done
    void clear(void);
    void skipLine(int); //done
    void write(std::string, byte); //done //string w/ foreground colour
    void write(std::string, byte, byte); //done //string w/ foreground and background colour
    void writeLine(std::string, byte); //done //string w/ foreground colour
    void writeLine(std::string, byte, byte); //done //string w/ foreground and background colour
};

