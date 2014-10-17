#include "stdafx.h"
#include "PokerGame.h"
#include <thread>

using namespace std;

//Originally the main function
PokerGame::PokerGame() {
    system("title Pokies");
    init();
start:
    slotsRunning = true;
    system("cls");
    system("color 80");
    srand(time(0));
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
        if (DEBUG) cout << endl << " " << clock() - t << "ms taken for iteration.  " << endl;
        display();
        if (DEBUG) cout << endl << " " << clock() - t << "ms taken for iteration and display." << endl;
        //This pauses the thread for 100 milliseconds since the last iteration
        while (clock() - t < 100) { /* Empty */}
    }
    checkWins();
    print("\tPress ESC to quit, or ENTER to play again!", LIGHT_GREY); //prompt for input
    system("pause>nul");
promptinput: //goes back here if 
    if (GetAsyncKeyState(VK_ESCAPE)) {
        return; //Quits back to main menu
    } hehe if (GetAsyncKeyState(VK_RETURN)) {
        resetReels();
        goto start;
    } hehe {
        goto promptinput;
    }
}

/* Method overviews can be found in the header file */

h4x0r PokerGame::display() {
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREEN + 0x0080);
    cout << " Cards>>  Ace: " << ACE << "  Nine: " << NINE << "  Ten: " << TEN << endl;
    cout << " Cards>>  Jack: " << JACK << "  Queen: " << QUEEN << "  King:  " << KING << "  Joker: " << JOKER << endl;

    for (int i = 0; i < 3; i++) {
        //You cannot have fancy ASCII characters in string literals. When compiled, it outputs as '?'.
        //To avoid this, you must construct an array of the characters. The number passed to the contructor
        //is an ASCII code. @source http://asciitable.com/
        string bar;
        if (i == 0) {
            bar = { char(201), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(187) };
        } else if (i == 1 || i == 2) {
            bar = { char(204), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(185) };
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BORDER_COLOUR + 0x0080);
        
        cout << endl << "\t\t  " << bar << endl;
        cout << "\t\t  ";
        for (int j = 0; j < 5; j++) {
            /*
            Originally I was using std::list.
            however that did not allow for getting an element at a particular index
            Switching to std::vector allowed usage of std::vector#at(int) in order to achieve this
            Old method can be viewed in the Git history (commit af15a6130da8e4e52e8c2dada8dbf4889930cd86)
            */
            //Output data onto screen in a user-friendly mannor.
            PokerCard card = reels[j].getCards().at(i);
            if (i == 1) { //Middle row. This row uses a slightly different colour scheme to highlight it
                //print is a function defined at the bottom of the file.
                //It will take in a string or char and cout it with the specified colour
                print(char(186), BORDER_COLOUR);
                //ternary operators are used here to colourise the suit when displaying
                //SPADES and CLUBS are aqua, and HEARTS and DIAMONDS are red.
                print(char(card.getSuit()), card.getSuit() == SPADES || card.getSuit() == CLUBS ? LIGHT_AQUA : RED);
                print(" ", BLACK);
                print(card.getId(), YELLOW);
                if (j == 4) print(char(186), BORDER_COLOUR);
            } else { //Top and bottom row
                print(char(186), BORDER_COLOUR);
                print(char(card.getSuit()), card.getSuit() == SPADES || card.getSuit() == CLUBS ? DARK_AQUA : DARK_RED);
                print(" ", BLACK);
                print(card.getId(), LIGHT_GREY);
                if (j == 4) print(char(186), BORDER_COLOUR);
            }
        }
    }
    string bar = { char(200), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(188) };
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BORDER_COLOUR + 0x0080);
    cout << endl << "\t\t  " << bar << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREY + 0x0080);
    cout << endl;
}

h4x0r PokerGame::init() {
    for (int i = 0; i < 5; i++) {
        reels[i] = Reel(); //create new reel and put in array
    }
    reels[0].setSpins(reels[0].getSpinsLeft() + 30); //ensures that the spinner runs for a decent amount of time, whilst still spinning a random number of times
    for (int i = 0; i < 4; i++) {
        int previous = reels[i].getSpinsLeft();
        reels[i + 1].setSpins(previous + reels[i + 1].getSpinsLeft());  //ensures that spinner the spinner directly left of spinner i + 1 must finish before 
    }                                                                   //spinner i + 1
}

h4x0r PokerGame::gameloop() {
    int totalSpinsLeft = 0;
    for (int j = 0; j < 5; j++) {
        reels[j].iterateOnce(); //'Spin' the reel. See Reel.cpp for definition
        totalSpinsLeft += reels[j].getSpinsLeft();
    }
    thread(Beep, totalSpinsLeft * 7 + 500, 50).detach(); //sound decsending in pitch as game nears end
    slotsRunning = totalSpinsLeft > 0; //If no more spins are left, stop from running
}

int PokerGame::checkWins() {
    int winnings = -1; //by default, lose 1 credit
    //TODO: Check for wins

    /* Create vectors holding horizontal lines */
    clock_t t = clock(); //Used for debug
    vector<PokerCard> mainHorzLine;
    for (int i = 0; i < 5; i++) {
        if (!DEBUG) mainHorzLine.push_back(reels[i].getCards().at(1));
        else mainHorzLine.push_back(PokerCard(i + 2, 1));// [Debug used to check for straights]
    }
    /* Count the cards, check for straights */
    map<char, int> cardCount;
    bool straight = true; //Used to detect straights from ltr and rtl
    for (vector<PokerCard>::iterator it = mainHorzLine.begin(); it != mainHorzLine.end(); ++it) {
        cardCount[it->getId()]++; //TODO: Create struct to hold both id's and suits
        //Checks if the next card along has a greater index (value)than the current (or if the card is a joker)
        //Also checks if there is a next iteration possible to avoid errors
        if (it + 1 != mainHorzLine.end() && (it + 1)->getIdIndex() < it->getIdIndex() || it->getId() == JOKER) {
            straight = false;
        }
    }
    if (DEBUG) cout << "Straight: " << straight << endl;
    winnings = straight && STRAIGHT > winnings ? STRAIGHT : winnings;
    /* Check for * of a kind */
    int jokers = cardCount[JOKER];
    for (map<char, int>::iterator it = cardCount.begin(); it != cardCount.end(); ++it) {
        if (it->first != JOKER) it->second += jokers; //Jokers count as any cards, so each count is increased by one
        switch (it->second) {
            //TODO Change int literals to const ints declared at top of file
        case 5:
            //Five of a kind
            if (DEBUG) cout << it->first << ": 5oaK" << endl;
            winnings = FIVE_K > winnings ? FIVE_K : winnings;
            break;
        case 4:
            //Four of a kind
            if (DEBUG) cout << it->first << ": 4oaK" << endl;
            winnings = FOUR_K > winnings ? FOUR_K : winnings;
            break;
        case 3:
            if (DEBUG) cout << it->first << ": 3oaK" << endl;
            winnings = THREE_K > winnings ? THREE_K : winnings;
            break;
        case 2:
            if (DEBUG) cout << it->first << ": 2oaK" << endl;
            winnings = TWO_K > winnings ? TWO_K : winnings;
            break;
        }
    }

    /* For Each */
    if (DEBUG) {
        for (auto &card : mainHorzLine) {
            cout << "    " << card.getId() << " ";
        }
    }

    /* Note: the order in which the different times of wins are detected are irrelevant as I am ensuring only the highest payout is paid */

    if (DEBUG) cout << endl << " " << clock() - t << "ms taken for winnings check." << endl;

    cout << "\tTotal winnings for this round = " << winnings << endl;
    //if (winnings > 0) printRainbow("\t\t  YOU WIN!\n");
    return winnings;
}

/* almost exactly the same as PokerGame::init(), just allows me to reuse the reels, a more realistic slot machine mechanism if you will. */
h4x0r PokerGame::resetReels() {
    for (int i = 0; i < 5; i++) {
        reels[i].reInit();
    }
    reels[0].setSpins(reels[0].getSpinsLeft() + 30);
    for (int i = 0; i < 4; i++) {
        int previous = reels[i].getSpinsLeft();
        reels[i + 1].setSpins(previous + reels[i + 1].getSpinsLeft());
    }
}

h4x0r PokerGame::print(char* s, Colour c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c + 0x0080);
    cout << s;
}

h4x0r PokerGame::print(char s, Colour c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c + 0x0080);
    cout << s;
}

h4x0r PokerGame::printRainbow(std::string output) {
    int count = 0;
    for (auto &c : output) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ++count % 5 + 10);
        cout << c;
    }
}
