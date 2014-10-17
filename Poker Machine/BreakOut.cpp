#include "stdafx.h"
#include "BreakOut.h"

BreakOut::BreakOut() {
    system("title BreakOut!");
    buffer.clear();
    buffer.writeCentered("HELLO WORLD, CAN YOU HEAR ME?", F_BLACK, B_GREY);
    buffer.print();
    std::cin.get();
}
