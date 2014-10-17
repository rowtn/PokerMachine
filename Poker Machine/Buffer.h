#pragma once

#include "Windows.h"
#include <list>

const byte F_BLACK = 0x0000, F_DARK_BLUE = 0x0001, F_DARK_GREEN = 0x0002, F_DARK_AQUA = 0x0003, F_DARK_RED = 0x0004, F_PURPLE = 0x0005, F_GOLD = 0x0006, F_WHITE = 0x0007, F_GREY = 0x0008,
    F_LIGHT_GREEN = 0x000a, F_LIGHT_AQUA = 0x000b, F_LIGHT_RED = 0x000c, F_PINK = 0x000d, F_YELLOW = 0x000e, F_BRIGHT_WHITE = 0x000f,
    B_BLACK = 0x000, B_DARK_BLUE = 0x010, B_DARK_GREEN = 0x020, B_DARK_AQUA = 0x030, B_DARK_RED = 0x040, B_PURPLE = 0x050, B_GOLD = 0x060, B_WHITE = 0x070, B_GREY = 0x080,
    B_LIGHT_GREEN = 0x0a0, B_LIGHT_AQUA = 0x0b0, B_LIGHT_RED = 0x0c0, B_PINK = 0x0d0, B_YELLOW = 0x0e0, B_BRIGHT_WHITE = 0x0f0;

class Buffer {
private:
    HANDLE handle;

    COORD size;
    COORD coord;
    SMALL_RECT region;

    CHAR_INFO buffer[3600];

    int column = 0, row = 0;

    byte defaultColour;
public:
    Buffer();
    Buffer(int, int, byte);
    void print(void);
    void write(std::string);
    void writeLine(std::string);
    void clear(void);
    void skipLine(int);
    void write(std::string, byte);
    void write(std::string, byte, byte);
    void writeLine(std::string, byte);
    void writeLine(std::string, byte, byte);
    void writeCentered(std::string, byte, byte);
    void writeAt(char, int, int, byte, byte);
};
