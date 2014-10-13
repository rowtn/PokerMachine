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
        list<PokerCard>::iterator it;
        int i; //used to only display only 3 cards of the reel at any given time
        for (i = 0, it = reelCards.begin(); i < 3 && it != reelCards.end(); i++, ++it) {
            cout << it->getId() << " | " << it->getSuit() << endl;
        }
        Sleep(100);
    }
    system("pause");
	return 0;
}
