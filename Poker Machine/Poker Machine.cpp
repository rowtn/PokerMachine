// Poker Machine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PokerCard.h"
#include "Reel.h"
#include <Windows.h>
#include <vector>
#include <time.h>

using namespace std;

/* Method prototypes */
void display(void);     //display cards on screen
void init(void);        //initialize slot machine
void gameloop(void);    //calculations to be done every frame
int checkWins(void);    //Checks for wins, returns credits won

Reel reels[5];
bool slotsRunning = true;

int _tmain(int argc, _TCHAR* argv[]) {
    system("color a");
start:
    slotsRunning = true;
    system("cls");
    srand(time(0));
    init();
    while (slotsRunning) {
        clock_t t = clock(); //Used for debug
        /*
            The following two lines are used for clearing the screen.
            It puts the system cursor at the start of the screen,
            any output after (cout << "Hello world") will override the old characters on the screen.
            Similar to pressing 'Ins' on your keyboard, and typing over a sentance.
            This works in this case as it is only neccesary to over-write characters on the screen.
            However system("cls") is still required when a full clear screen is needed. This is essentially a little hack.
            */
        COORD position = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
        gameloop();
        display();
        cout << endl << " " << clock() - t << "ms taken for iteration." << endl;
        Sleep(100);
    }
    checkWins();
    cout << "Press any key to play again!" << endl;
    system("pause>nul");
    goto start;
	return 0;
}

/* Method overviews can be found at the top of the file */
void display() {
    cout << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            PokerCard card = reels[j].getCards().at(i);                             //Originally I was using std::list.
            cout << " |" << card.getId() << " | " << char(card.getSuit()) << "|";   //however that did not allow for getting an element at a particular index
        }                                                                           //Switching to std::vector allowed usage of std::vector#at(int) in order to achieve this
        cout << endl;                                                               //Old method can be viewed in the Git history (commit af15a6130da8e4e52e8c2dada8dbf4889930cd86)
    }
    for (int i = 0; i < 5; i++) {
        cout << "   " << reels[i].getSpinsLeft() << "    ";
    }
}

void init() {
    for (int i = 0; i < 5; i++) {
        reels[i] = Reel(); //create new reel and put in array
    }
    reels[0].setSpins(reels[0].getSpinsLeft() + 30); //ensures that the spinner runs for a decent amount of time, whilst still spinning a random number of times
    for (int i = 0; i < 4; i++) {
        int previous = reels[i].getSpinsLeft();
        reels[i + 1].setSpins(previous + reels[i + 1].getSpinsLeft());  //ensures that spinner the spinner directly left of spinner i + 1 must finish before 
    }                                                                   //spinner i + 1
}

void gameloop() {
    int totalSpinsLeft = 0;
    for (int j = 0; j < 5; j++) {
        reels[j].iterateOnce();
        totalSpinsLeft += reels[j].getSpinsLeft();
    }
    slotsRunning = totalSpinsLeft > 0; //If no more spins are left, stop from running
}

int checkWins() {
    //TODO: Check for wins
    vector<PokerCard> lineOne, lineTwo, lineThree;
    for (int i = 0; i < 5; i++) {
        lineOne.push_back(reels[i].getCards().at(0));
        lineTwo.push_back(reels[i].getCards().at(1));
        lineThree.push_back(reels[i].getCards().at(2));
    }
    for (int i = 0; i < 5; i++) {
        cout << lineOne.at(i).getId() << "       ";
    }
    cout << endl;
    for (int i = 0; i < 5; i++) {
        cout << lineTwo.at(i).getId() << "       ";
    }
    cout << endl;
    for (int i = 0; i < 5; i++) {
        cout << lineThree.at(i).getId() << "       ";
    }
    return -1;
}
