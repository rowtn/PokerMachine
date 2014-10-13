// Poker Machine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"
#include <Windows.h>

using namespace std;

/* Method prototypes */
void display(void); //display cards on screen
void init(void); //initialize slot machine
void gameloop(void); //calculations to be done every frame

Reel reels[5];

int _tmain(int argc, _TCHAR* argv[]) {
    srand(time(0));
    system("color a");
    Reel reel = Reel();
    list<PokerCard> reelCards;
    while (true) {
        COORD position = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
        gameloop();
        display();
        Sleep(100);
    }
    system("pause");
	return 0;
}

void display() {
    cout << endl;
    for (int j = 0; j < 5; j++) {
        Reel reel = reels[j];
        list<PokerCard> reelCards = reel.getCards();
        list<PokerCard>::iterator it;
        int i, x = j * 9; //used to only display only 3 cards of the reel at any given time. int j is a cached value
        //used for ensuring horizontal display. (Not caching it would mean that the same calculation is carried out
        //multiple times redundantly
        for (i = 0, it = reelCards.begin(); i < 3 && it != reelCards.end(); i++, ++it) {
            COORD position = { x, i };
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position); //This line and the one above allows me to output the cards horizontally.
            cout << " |" << it->getId() << " | " << char(it->getSuit()) << "|" << endl; //This was the simplest and most effecient solution as std::list does not have 
        }                                                                               //the capability to get a specific value at an index
    }
}

void init() {
    for (int i = 0; i < 5; i++) {
        reels[i] = Reel(); //create new reel and put in array
    }
}

void gameloop() {
    for (int j = 0; j < 5; j++) {
        reels[j].iterateOnce();
        //TODO: Sequential stopping of reels
    }
}
