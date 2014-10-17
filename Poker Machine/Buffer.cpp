#include "stdafx.h"
#include "Buffer.h"


Buffer::Buffer(int x, int y) {
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    size.X = x;
    size.Y = y;
    coord = { 0, 0 };
    region = { 0, 0, x - 1, y - 1 };
    SetConsoleScreenBufferSize(handle, size);
}

void Buffer::write(std::string s) {
    for (auto &c : s) {
        buffer[column + 30 * row].Char.AsciiChar = char(c);
        buffer[column + 30 * row].Attributes = 0x0007;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::writeLine(std::string s) {
    column = size.X;
    for (auto &c : s) {
        buffer[column + 30 * row].Char.AsciiChar = char(c);
        buffer[column + 30 * row].Attributes = 0x0007;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::print() {
    WriteConsoleOutputA(handle, buffer, size, coord, &region);
}
