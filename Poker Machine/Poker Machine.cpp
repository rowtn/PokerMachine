// Poker Machine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"
#include <Windows.h>
#include <vector>

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
    vector<PokerCard> reelCards;
    while (true) {
        COORD position = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
        gameloop();
        display();
        Sleep(200);
    }
    system("pause");
	return 0;
}

void display() {
    cout << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            PokerCard card = reels[j].getCards().at(i);                             //Originally I was using std::list.
            cout << " |" << card.getId() << " | " << char(card.getSuit()) << "|";   //t, however that did not allow for getting an element at a particular index
        }                                                                           //Switching to std::vector allowed usage of std::vector#at(int) in order to achieve this
        cout << endl;                                                               //Old method can be viewed in the Git history (commit af15a6130da8e4e52e8c2dada8dbf4889930cd86)
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
