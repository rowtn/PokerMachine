#pragma once

#include "PokerCard.h"
#include <list>

class Reel {
private:
    std::list<PokerCard> reelCards;
public:
    Reel(); //constructor
    ~Reel(); //destructor

    std::list<PokerCard> getCards() {
        return reelCards;
    }
    void repopulateReel(); //Fill reelCards with random cards
    void iterateOnce(); //Effectively rotates the reel
};

