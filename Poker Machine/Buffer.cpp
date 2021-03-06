#include "stdafx.h"
#include "Buffer.h"

Buffer::Buffer(int x, int y, byte bgDefault) {
    handle = GetStdHandle(STD_OUTPUT_HANDLE); //console output handle
    //init size
    size.X = x;
    size.Y = y;
    buffer = new CHAR_INFO[x * y];
    coord = { 0, 0 };
    region = { 0, 0, x - 1, y - 1 }; //documentation suggested to do this
    SetConsoleScreenBufferSize(handle, size); //ensure console buffer size is consistant with this buffer class
    defaultColour = bgDefault;
    clear(); //write blanks to buffer
}

void Buffer::write(std::string s) {
    for (auto &c : s) { //Gets every char in string
        if (c == '\n') { //if the char is \n, skip to next line
            row++;
            column = 0;
            continue;
        }
        //this small algorithm allows for a 1D CHAR_INFO array to be used instead of a 2D
        //It gets the current column, and row, and calculates which element in the array needs to be modified
        buffer[column + size.X * row].Char.AsciiChar = c;
        buffer[column + size.X * row].Attributes = defaultColour;
        column++;
        if (column % size.X == 0) { //if reached end of column, increment row counter, reset column counter
            column = 0;
            row++;
        }
    }
}

void Buffer::writeLine(std::string s) {
    column = size.X; //go to end of row
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        //same as Buffer::write(std::string)
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = defaultColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::print() {
    WriteConsoleOutputA(handle, buffer, size, coord, &region); //Dumps buffer into console screen
}

void Buffer::skipLine(int skips) {
    if (row + skips > size.Y) {
        throw "Text outside of buffer"; //throws error if skipping lines beyond the buffer size
    }
    row += skips;
    column = 0;
}

void Buffer::write(std::string s, byte colour) {
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        //same as above
        buffer[column + size.X * row].Char.AsciiChar = c;
        buffer[column + size.X * row].Attributes = colour; //default bg colour will be black
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::writeCentered(std::string s, byte colour) {
    row++;
    column = (size.X / 2) - (s.length() / 2) + 1; //Calculates where the cursor must be set in order for the line to be centered
    for (auto &c : s) {
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = colour;
        column++;
        if (column % size.X == 0) {
            column = (size.X / 2) - (s.length() / 2) + 1;
            row++;
        }
    }
}

void Buffer::writeLine(std::string s, byte colour) {
    column = size.X;
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = colour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::clear() {
    for (int i = 0; i < size.X * size.Y; i++) { //loops through every element in buffer and sets it to a space (' ') with the default colour
        buffer[i].Char.AsciiChar = char(0);
        buffer[i].Attributes = defaultColour;
    }
    column = 0;
    row = 0;
}

//writes char at given location
void Buffer::writeAt(char c, int x, int y, byte colour) {
    buffer[x + size.X * y].Char.AsciiChar = c;
    buffer[x + size.X * y].Attributes = colour;
}

//set the cursor position to desired values. Useful in 'BreakOut!'
void Buffer::setCursorPosition(int x, int y) {
    column = x;
    row = y;
}

IntPair Buffer::getCursorPosition() {
    return IntPair(column, row);
}
