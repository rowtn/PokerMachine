// Poker Machine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {
    srand(time(0));
    cout << "Hello, world" << endl;
    Reel reel = Reel();
    list<PokerCard> reelCards = reel.getCards();
    for (list<PokerCard>::iterator it = reelCards.begin(); it != reelCards.end(); ++it) {
        cout << it->getId() << "|" << it->getSuit() << endl;
    }
    cout << endl << "====================" << endl;
    reel.iterateOnce();
    reelCards = reel.getCards();
    for (list<PokerCard>::iterator it = reelCards.begin(); it != reelCards.end(); ++it) {
        cout << it->getId() << "|" << it->getSuit() << endl;
    }
    cout << endl << "====================" << endl;
    reel.iterateOnce();
    reelCards = reel.getCards();
    for (list<PokerCard>::iterator it = reelCards.begin(); it != reelCards.end(); ++it) {
        cout << it->getId() << "|" << it->getSuit() << endl;
    }
    system("pause");
	return 0;
}
