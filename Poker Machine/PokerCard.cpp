#include "stdafx.h"
#include "PokerCard.h"

/* Definitions */

/**
    @param x the card type (e.g. ACE, KINGS)
    @param  y the suit of the card
*/
PokerCard::PokerCard(char x, int y) {
    id = x;
    suit = y;
}

/**
    @param a the other PokerCard to compare to
*/
bool PokerCard::operator==(PokerCard& a) {
    return (id == a.id && suit == a.suit);
}

/**
  @return a random valid PokerCard
  */
PokerCard PokerCard::random() {
    int s = rand() % 4; //Originally I had an enum for the Suits, but C++ does not support getting an enum value with an index
    switch (s) {        //So instead I used ints to do the job. This is the most effiecient yet readable way of doing what I wanted
    case 0:
        s = SPADES;
        break;
    case 1:
        s = HEARTS;
        break;
    case 2:
        s = DIAMONDS;
        break;
    case 3:
        s = CLUBS;
        break;
    }
    return PokerCard(possibleChars[rand() % 7], s);
}

char PokerCard::getId() {
    return id;
}

int PokerCard::getSuit() {
    return suit;
}
