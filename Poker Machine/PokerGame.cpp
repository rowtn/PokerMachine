#include "stdafx.h"
#include "PokerGame.h"

using namespace std;

PokerGame::PokerGame() {

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
        cout << endl << " " << clock() - t << "ms taken for iteration." << endl;
        display();
        cout << endl << " " << clock() - t << "ms taken for iteration and display." << endl;
        while (clock() - t < 100)
        {
            //md_5
        }
    }
    checkWins();
    cout << "Press any key to play again!" << endl;
    system("pause>nul");
    goto start;

}

/* Method overviews can be found in the header file */

void PokerGame::display() {
    cout << endl;
    cout << " Cards>>  Ace: " << ACE << "  Nine: " << NINE << "  Ten: " << TEN << endl;
    cout << " Cards>>  Jack: " << JACK << "  Queen: " << QUEEN << "  King:  " << KING << "  Joker: " << JOKER << endl << endl;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (i == 0 || i == 2) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 'd');
            }
            PokerCard card = reels[j].getCards().at(i);
            //TODO: Make output prettier  
            /*
            Originally I was using std::list.
            however that did not allow for getting an element at a particular index
            Switching to std::vector allowed usage of std::vector#at(int) in order to achieve this
            Old method can be viewed in the Git history (commit af15a6130da8e4e52e8c2dada8dbf4889930cd86)
            */
            cout << " |" << char(card.getSuit()) << " " << card.getId() << "|\t";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
        cout << endl << endl;
    }
    for (int i = 0; i < 5; i++) {
        cout << "   " << reels[i].getSpinsLeft() << "    ";
    }
}

void PokerGame::init() {
    for (int i = 0; i < 5; i++) {
        reels[i] = Reel(); //create new reel and put in array
    }
    reels[0].setSpins(reels[0].getSpinsLeft() + 30); //ensures that the spinner runs for a decent amount of time, whilst still spinning a random number of times
    for (int i = 0; i < 4; i++) {
        int previous = reels[i].getSpinsLeft();
        reels[i + 1].setSpins(previous + reels[i + 1].getSpinsLeft());  //ensures that spinner the spinner directly left of spinner i + 1 must finish before 
    }                                                                   //spinner i + 1
}

void PokerGame::gameloop() {
    int totalSpinsLeft = 0;
    for (int j = 0; j < 5; j++) {
        reels[j].iterateOnce(); //'Spin' the reel. See Reel.cpp for definition
        totalSpinsLeft += reels[j].getSpinsLeft();
    }
    slotsRunning = totalSpinsLeft > 0; //If no more spins are left, stop from running
}

int PokerGame::checkWins() {
    int winnings = -1; //by default, lose 1 credit
    //TODO: Check for wins

    /* Create vectors holding horizontal lines */
    clock_t t = clock(); //Used for debug
    vector<PokerCard> lineOne, lineTwo, lineThree;
    for (int i = 0; i < 5; i++) {
        lineOne.push_back(reels[i].getCards().at(0));
        lineTwo.push_back(reels[i].getCards().at(1));
        //lineTwo.push_back(PokerCard(7 - i + 1, 1));// [Debug used to check for straights]
        lineThree.push_back(reels[i].getCards().at(2));
    }
    /* Count the cards, check for straights */
    map<char, int> cardCount;
    bool straightA = true, straightB = true; //Used to detect straights from ltr and rtl
    for (vector<PokerCard>::iterator it = lineTwo.begin(); it != lineTwo.end(); ++it) {
        cardCount[it->getId()]++; //TODO: Create struct to hold both id's and suits
        //Checks if the next card along has a greater index (value)than the current (or if the card is a joker)
        //Also checks if there is a next iteration possible to avoid errors
        if (it + 1 != lineTwo.end() && (it + 1)->getIdIndex() < it->getIdIndex() || it->getId() == JOKER) {
            straightA = false;
        }
        if (it + 1 != lineTwo.end() && (it + 1)->getIdIndex() > it->getIdIndex() || it->getId() == JOKER) {
            straightA = false;
        }
    }
    bool straight = straightA || straightB; //if either straights are present, => win straight
    cout << "Straight: " << straight << endl;
    winnings = straight && 12 > winnings ? 12 : winnings;
    /* Check for * of a kind */
    int jokers = cardCount[JOKER];
    for (map<char, int>::iterator it = cardCount.begin(); it != cardCount.end(); ++it) {
        if (it->first != JOKER) it->second += jokers; //Jokers count as any cards, so each count is increased by one
        switch (it->second) {
            //TODO Change int literals to const ints declared at top of file
        case 5:
            //Five of a kind
            cout << it->first << ": 5oaK" << endl;
            winnings = 15 > winnings ? 15 : winnings;
            break;
        case 4:
            //Four of a kind
            cout << it->first << ": 4oaK" << endl;
            winnings = 6 > winnings ? 6 : winnings;
            break;
        case 3:
            cout << it->first << ": 3oaK" << endl;
            winnings = 3 > winnings ? 3 : winnings;
            break;
        case 2:
            cout << it->first << ": 2oaK" << endl;
            winnings = 1 > winnings ? 1 : winnings;
            break;
        }
    }

    /* Note: the order in which the different times of wins are detected are irrelevant as I am ensuring only the highest payout is paid */

    cout << endl << " " << clock() - t << "ms taken for winnings check." << endl;

    cout << "Total winnings for this round = " << winnings << endl;
    return -1;
}
