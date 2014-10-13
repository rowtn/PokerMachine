// Poker Machine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"
#include <Windows.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {
    srand(time(0));
    system("color a");
    Reel reel = Reel();
    list<PokerCard> reelCards;
    while (true) {
        COORD position = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
        reel.iterateOnce();
        reelCards = reel.getCards();
        for (list<PokerCard>::iterator it = reelCards.begin(); it != reelCards.end(); ++it) {
            cout << it->getId() << "|" << it->getSuit() << endl;
        }
        Sleep(100);
    }
    system("pause");
	return 0;
}
