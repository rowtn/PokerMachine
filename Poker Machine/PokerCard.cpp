#include "stdafx.h"
#include "PokerCard.h"

/* Definitions */

/**
    @param x the card type (e.g. ACE, KINGS)
    @param  y the suit of the card
*/
PokerCard::PokerCard(char x, Suit y) {
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
    Suit s = static_cast<Suit>(rand() % DIAMONDS);
    return PokerCard(possibleChars[rand() % 7], s);
}
