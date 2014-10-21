#include "stdafx.h"
#include "Buffer.h"


Buffer::Buffer(int x, int y, byte bgDefault) {
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    size.X = x;
    size.Y = y;
    coord = { 0, 0 };
    region = { 0, 0, x - 1, y - 1 };
    SetConsoleScreenBufferSize(handle, size);
    defaultColour = bgDefault;
    clear();
}

h4x0r Buffer::write(std::string s) {
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = 0x0007;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

h4x0r Buffer::writeLine(std::string s) {
    column = size.X;
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = 0x0007;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

h4x0r Buffer::print() {
    WriteConsoleOutputA(handle, buffer, size, coord, &region);
}

h4x0r Buffer::skipLine(int skips) {
    if (row + skips > size.Y) {
        throw "Text outside of buffer";
    }
    row += skips;
    column = 0;
}

h4x0r Buffer::write(std::string s, byte foregroundColour) {
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = foregroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

h4x0r Buffer::write(std::string s, byte foregroundColour, byte backgroundColour) {
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = foregroundColour + backgroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

h4x0r Buffer::writeLine(std::string s, byte foregroundColour) {
    column = size.X;
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = foregroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

h4x0r Buffer::writeLine(std::string s, byte foregroundColour, byte backgroundColour) {
    column = size.X;
    for (auto &c : s) {
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        }
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = foregroundColour + backgroundColour;
        column++;
        if (column % size.X == 0) {
            column = 0;
            row++;
        }
    }
}

h4x0r Buffer::clear() {
    for (auto &c : buffer) {
        c.Char.AsciiChar = char(0);
        c.Attributes = defaultColour;
    }
    column = 0;
    row = 0;
}

h4x0r Buffer::writeCentered(std::string s, byte foregroundColour, byte backgroundColour) {
    row++;
    column = (size.X / 2) - (s.length() / 2) + 1;
    for (auto &c : s) {
        buffer[column + size.X * row].Char.AsciiChar = char(c);
        buffer[column + size.X * row].Attributes = foregroundColour + backgroundColour;
        column++;
        if (column % size.X == 0) {
            column = (size.X / 2) - (s.length() / 2) + 1;
            row++;
        }
    }
}

h4x0r Buffer::writeAt(char c, int x, int y, byte foregroundColour, byte backgroundColour) {
    buffer[x + size.X * y].Char.AsciiChar = c;
    buffer[x + size.X * y].Attributes = foregroundColour + backgroundColour;
}
