#pragma once

#include "Windows.h"
#include <list>

/*
The underlying theory to the Buffer is very simple.
An array of char wrappers (CHAR_INFO), holding all the characters
that will be written into the console is filled with the desired content,
and then dumbed into the output, in this case, console.
The bytes below represent colours. 0x000y, where y is any hex value representing a colour. 
The hex values are identical to those used when running the batch command 'color'.
0x00ab where a is the background colour, and b is the foreground
*/

const byte F_BLACK = 0x0000, F_DARK_BLUE = 0x0001, F_DARK_GREEN = 0x0002, F_DARK_AQUA = 0x0003, F_DARK_RED = 0x0004, F_PURPLE = 0x0005, F_GOLD = 0x0006, F_WHITE = 0x0007, F_GREY = 0x0008,
    F_LIGHT_GREEN = 0x000a, F_LIGHT_AQUA = 0x000b, F_LIGHT_RED = 0x000c, F_PINK = 0x000d, F_YELLOW = 0x000e, F_BRIGHT_WHITE = 0x000f,
    B_BLACK = 0x000, B_DARK_BLUE = 0x010, B_DARK_GREEN = 0x020, B_DARK_AQUA = 0x030, B_DARK_RED = 0x040, B_PURPLE = 0x050, B_GOLD = 0x060, B_WHITE = 0x070, B_GREY = 0x080,
    B_LIGHT_GREEN = 0x0a0, B_LIGHT_AQUA = 0x0b0, B_LIGHT_RED = 0x0c0, B_PINK = 0x0d0, B_YELLOW = 0x0e0, B_BRIGHT_WHITE = 0x0f0;

class Buffer {
private:
    //HANDLE for the console
    HANDLE handle;

    //Size of the screen
    COORD size;
    //Where to start writing chars from CHAR_INFO to the screen
    COORD coord;
    //screen region
    SMALL_RECT region;

    //the buffer
    CHAR_INFO buffer[3600];

    //essentially x-y coords for writing into buffer
    int column = 0, row = 0;

    //the default colour to be used when no colour args are passed
    byte defaultColour;
public:
    Buffer();
    //int,int = size of screen, byte = default colour
    Buffer(int, int, byte);
    //print buffer to screen
    void print(void);

    //Various 'write' methods
    //char/std::string refers to what is being put into the buffer
    //the bytes are the colours for the input char/string
    void write(std::string);
    void writeLine(std::string);
    void write(std::string, byte);
    void writeLine(std::string, byte);
    void writeCentered(std::string, byte);
    void writeAt(char, int, int, byte);

    //fills buffer with empty CHAR_INFOs
    void clear(void);
    //skips int amount of lines
    void skipLine(int);
    //Set cursor position (x, y)
    void setCursorPosition(int, int);
    IntPair getCursorPosition();
};
