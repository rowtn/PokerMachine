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
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
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
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
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

void Buffer::skipLine(int skips) {
    if (row + skips > size.Y) {
        throw "Text outside of buffer";
    }
    row += skips;
    column = 0;
}

void Buffer::write(std::string s, byte foregroundColour) {
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + 30 * row].Char.AsciiChar = char(c);
        buffer[column + 30 * row].Attributes = foregroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::write(std::string s, byte foregroundColour, byte backgroundColour) {
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + 30 * row].Char.AsciiChar = char(c);
        buffer[column + 30 * row].Attributes = foregroundColour + backgroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::writeLine(std::string s, byte foregroundColour) {
    column = size.X;
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + 30 * row].Char.AsciiChar = char(c);
        buffer[column + 30 * row].Attributes = foregroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::writeLine(std::string s, byte foregroundColour, byte backgroundColour) {
    column = size.X;
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + 30 * row].Char.AsciiChar = char(c);
        buffer[column + 30 * row].Attributes = foregroundColour + backgroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

void Buffer::clear() {
    for (auto &cInfo : buffer) {
        CHAR_INFO c;
        c.Char.AsciiChar = char(0);
        c.Attributes = 0x0000;
        cInfo = c;
    }
    column = 0;
    row = 0;
}
