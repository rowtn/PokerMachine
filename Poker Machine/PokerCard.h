#pragma once

/* Prototypes */

/*enum Suit {
    SPADES = 6, HEARTS = 3, CLUBS = 5, DIAMONDS = 4 //The numbers correlate to ascii codes (♠ ♥ ♣ ♦)
};*/

const int SPADES = 6, HEARTS = 3, CLUBS = 5, DIAMONDS = 4;

const char ACE = 'A', NINE = 'N', TEN = 'T', JACK = 'J', QUEEN = 'Q', KING = 'K', JOKER = 'j';
const char possibleChars[7] = { ACE, NINE, TEN, JACK, QUEEN, KING, JOKER };

class PokerCard {
private:
    char id;
    int suit;
public:
    PokerCard(char, int);
    bool operator==(PokerCard& a);
    static PokerCard random(); //static method so new card can be generated without needing an instance
    char getId();
    int getSuit();
};
