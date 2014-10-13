#pragma once

/* Prototypes */

enum Suit {
    SPADES, HEARTS, CLUBS, DIAMONDS
};

const char ACE = 'A', NINE = 'N', TEN = 'T', JACK = 'J', QUEEN = 'Q', KING = 'K', JOKER = 'j';
const char possibleChars[7] = { ACE, NINE, TEN, JACK, QUEEN, KING, JOKER };

class PokerCard {
private:
    char id;
    Suit suit;
public:
    PokerCard(char, Suit);
    bool operator==(PokerCard& a);
    PokerCard random();
};
