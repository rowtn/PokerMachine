#include "stdafx.h"
#include "PokerGame.h"
#include <thread>
#include <mmsystem.h>
#include <algorithm>
//Originally the main function

PokerGame::PokerGame() {
    system("title Pokies");
    init();
    system("cls");
    system("color 8a");
    std::cout << std::endl << std::endl << std::endl;
    delayedPrint("Welcome to Pokies! Enjoy your stay");
    delayedPrint("You will start out with 5 credits.");
    delayedPrint("You will earn coins for any wins!");
    delayedPrint("If you run out of credits you can");
    delayedPrint("trade a coin in for credits and keep");
    delayedPrint("playing. Or you could quit to the");
    delayedPrint("main menu and try BreakOut!");
    std::cout << std::endl;
    delayedPrint("Press any key to continue!");
    system("pause>nul");
start:
    slotsRunning = true;
    system("cls");
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
        if (DEBUG) std::cout << std::endl << " " << clock() - t << "ms taken for iteration.  " << std::endl;
        display();
        if (DEBUG) std::cout << std::endl << " " << clock() - t << "ms taken for iteration and display." << std::endl;
        //This pauses the thread for 100 milliseconds since the last iteration
        while (clock() - t < 100) { /* Empty */ }
    }
    credits--; //subract credit for turn
    if (checkWins() > 0) {
        //If won something
        //sound source: http://www.freesfx.co.uk/download/?type=mp3&id=3928
        PlaySound(TEXT("poker-win.wav"), NULL, SND_ASYNC);
    } else {
        //sound source: http://www.soundjay.com/misc/sounds/fail-trombone-01.mp3
        PlaySound(TEXT("poker-lose.wav"), NULL, SND_ASYNC);
    }
    if (credits <= 0 && coins <= 0) {
        std::cout << "\tSorry, you don't have any coins\n\t or credits. Press any key to return to the main menu";
        system("pause>nul");
        return; //return to menu
    }
    if (credits <= 0) {
        std::cout << "\tYou are out of credits.\n\tWould you like to trade a coin for 5 credits?" << std::endl;
        std::cout << "\tPress ENTER to add more credits,\n\tor ESCAPE to count your losses" << std::endl;
    promptcoininput: //goes back here if 
        if (GetAsyncKeyState(VK_ESCAPE)) {
            return; //Quits back to main menu
        } else if (GetAsyncKeyState(VK_RETURN)) {
            if (coins > 0) {
                coins--;
                credits += 5;
                resetReels();
                COORD position = { 0, 0 };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
                display();
                goto start;
            } else {
                std::cout << "\n\tSorry, you do not have enough coins to continue.\n\tPress any key to go back to the menu." << std::endl;
                COORD position = { 0, 0 };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
                display();
                system("pause>nul");
                return; //quit to main menu
            }
        } else {
            goto promptcoininput;
        }
    }
    print("\tPress ESC to quit, or ENTER to play again!", LIGHT_GREY); //prompt for input
    COORD position = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
    display();
    system("pause>nul");
promptinput: //goes back here if 
    if (GetAsyncKeyState(VK_ESCAPE)) {
        return; //Quits back to main menu
    } else if (GetAsyncKeyState(VK_RETURN)) {
        resetReels();
        goto start;
    } else {
        goto promptinput;
    }
}

/* Method overviews can be found in the header file */

void PokerGame::display() {
    std::cout << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREEN + 0x0080);
    std::cout << " Coins: " << coins << "    " << std::endl; //extra spaces added because of the method of clearing screen
    std::cout << " Credits: " << credits << "     " << std::endl;
    /* Card shorthand legend */
    std::cout << " Cards>>  Ace: " << ACE << "  Nine: " << NINE << "  Ten: " << TEN << std::endl;
    std::cout << " Cards>>  Jack: " << JACK << "  Queen: " << QUEEN << "  King:  " << KING << "  Joker: " << JOKER << std::endl;
    for (int i = 0; i < 3; i++) {
        //You cannot have fancy ASCII characters in string literals. When compiled, it outputs as '?'.
        //To avoid this, you must construct an array of the characters. The number passed to the contructor
        //is an ASCII code. @source http://asciitable.com/
        std::string bar;
        if (i == 0) {
            bar = { char(201), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(203), char(205), char(205), char(205), char(187) };
        } else if (i == 1 || i == 2) {
            bar = { char(204), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(206), char(205), char(205), char(205), char(185) };
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BORDER_COLOUR + 0x0080);

        std::cout << std::endl << "\t\t  " << bar << std::endl;
        std::cout << "\t\t  ";
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
                //It will take in a string or char and std::cout it with the specified colour
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
    std::string bar = { char(200), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(202), char(205), char(205), char(205), char(188) };
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BORDER_COLOUR + 0x0080);
    std::cout << std::endl << "\t\t  " << bar << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREY + 0x0080);
    std::cout << std::endl;
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
    std::thread(Beep, totalSpinsLeft * 7 + 500, 50).detach(); //sound decsending in pitch as game nears end
    slotsRunning = totalSpinsLeft > 0; //If no more spins are left, stop from running
}

int PokerGame::checkWins() {
    int winnings = 0; //by default, lose 1 credit
    //TODO: Check for wins

    /* Create vectors holding horizontal lines */
    clock_t t = clock(); //Used for debug
    std::vector<PokerCard> mainHorzLine;
    for (int i = 0; i < 5; i++) {
        if (!DEBUG) mainHorzLine.push_back(reels[i].getCards().at(1));
        else mainHorzLine.push_back(PokerCard(i + 2, 1));// [Debug used to check for straights]
    }
    /* Count the cards, check for straights */
    std::map<char, int> cardCount;
    bool straight = true; //Used to detect straights from ltr and rtl
    bool flushIO = true; //Flush in order
    bool flushUO = true; //unordered flush
    int royalFlushCards[5]; //store the card values (no suit). Used for unordered royal flush. the array gets sorted and then a flush is checked for
    int rFCCounter = 0; //counter
    for (std::vector<PokerCard>::iterator it = mainHorzLine.begin(); it != mainHorzLine.end(); ++it) {
        royalFlushCards[rFCCounter++] = it->getIdIndex();
        //if the first card isn't a 10, then a royal flush isn't possible
        if (it->getId() != TEN && it == mainHorzLine.begin()) flushIO = false;
        cardCount[it->getId()]++; //TODO: Create struct to hold both id's and suits
        //Checks if the next card along has a greater index (value)than the current (or if the card is a joker)
        //Also checks if there is a next iteration possible to avoid errors
        if (it + 1 != mainHorzLine.end() && (it + 1)->getIdIndex() <= it->getIdIndex() || it->getId() == JOKER) {
            straight = false;
        }
        //if the next card in the line isn't of the same suit, then a royal flush will not happen
        if (it + 1 != mainHorzLine.end() && (it + 1)->getSuit() != it->getSuit() || it->getId() != JOKER) {
            flushIO = false;
            flushUO = false;
        }
    }
    if (flushUO) { //if all suits are the same...
        std::sort(royalFlushCards, royalFlushCards + 5); //sort the array
        if (royalFlushCards[0] != 2) flushUO = false; //If the first in the sorted array isn't of index 2, i.e. the TEN card, it isn't a royal flush
        for (int i = 0; i < 4; i++) {
            if (royalFlushCards[i] > royalFlushCards[i + 1]) flushUO = false; //if the next card has a lower value, it isn't a royal flush
        }
    }
    if (DEBUG) std::cout << "Straight: " << straight << std::endl;
    //if win of whatever type, and winning prize is more than current winning amount, set winnings to new amount
    //ensures only the highest valued prize is given
    winnings = straight && STRAIGHT > winnings ? STRAIGHT : winnings;
    winnings = flushIO && straight && IO_R_FLUSH > winnings ? IO_R_FLUSH : winnings;
    winnings = flushUO && UO_R_FLUSH > winnings ? UO_R_FLUSH : winnings;
    /* Check for * of a kind */
    int jokers = cardCount[JOKER];
    for (std::map<char, int>::iterator it = cardCount.begin(); it != cardCount.end(); ++it) {
        if (it->first != JOKER) it->second += jokers; //Jokers count as any cards, so each count is increased by one
        switch (it->second) {
            //TODO Change int literals to const ints declared at top of file
        case 5:
            //Five of a kind
            if (DEBUG) std::cout << it->first << ": 5oaK" << std::endl;
            winnings = FIVE_K > winnings ? FIVE_K : winnings;
            break;
        case 4:
            //Four of a kind
            if (DEBUG) std::cout << it->first << ": 4oaK" << std::endl;
            winnings = FOUR_K > winnings ? FOUR_K : winnings;
            break;
        case 3:
            if (DEBUG) std::cout << it->first << ": 3oaK" << std::endl;
            winnings = THREE_K > winnings ? THREE_K : winnings;
            break;
        case 2:
            if (DEBUG) std::cout << it->first << ": 2oaK" << std::endl;
            winnings = TWO_K > winnings ? TWO_K : winnings;
            break;
        }
    }

    /* For Each */
    if (DEBUG) {
        for (auto &card : mainHorzLine) {
            std::cout << "    " << card.getId() << " ";
        }
    }

    /* Note: the order in which the different times of wins are detected are irrelevant as I am ensuring only the highest payout is paid */

    if (DEBUG) std::cout << std::endl << " " << clock() - t << "ms taken for winnings check." << std::endl;

    std::cout << "\tTotal winnings for this round = " << winnings << std::endl;
    //if (winnings > 0) printRainbow("\t\t  YOU WIN!\n");
    coins += winnings;
    return winnings;
}

/* almost exactly the same as PokerGame::init(), just allows me to reuse the reels, a more realistic slot machine mechanism if you will. */
void PokerGame::resetReels() {
    for (int i = 0; i < 5; i++) {
        reels[i].reInit();
    }
    reels[0].setSpins(reels[0].getSpinsLeft() + 30);
    for (int i = 0; i < 4; i++) {
        int previous = reels[i].getSpinsLeft();
        reels[i + 1].setSpins(previous + reels[i + 1].getSpinsLeft());
    }
}

/* prints string in specified colour with grey background */
void PokerGame::print(char* s, Colour c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c + 0x0080);
    std::cout << s;
}

void PokerGame::print(char s, Colour c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c + 0x0080);
    std::cout << s;
}

void PokerGame::printRainbow(std::string output) {
    int count = 0;
    for (auto &c : output) { //get all chars in stsring
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ++count % 5 + 10); //set output colour to whatever
        std::cout << c;
    }
}

void PokerGame::delayedPrint(std::string s) {
    COORD position = { 30 - s.length() / 2, };
    CONSOLE_SCREEN_BUFFER_INFO con;
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hcon, &con);
    position.Y = con.dwCursorPosition.Y;
    SetConsoleCursorPosition(hcon, position);
    bool skip = false;
    for (char c : s) {
        std::cout << c;
        clock_t t = clock();
        while (clock() - t < 50 && !skip) { /* Delay of 50ms */ }
        if (GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_RETURN)) skip = true;
    }
    std::cout << std::endl;
}
