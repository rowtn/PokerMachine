#pragma once

#include "PokerCard.h"
#include <vector>

class Reel {
private:
    std::vector<PokerCard> reelCards;
public:
    Reel(); //constructor
    ~Reel(); //destructor

    std::vector<PokerCard> getCards() {
        return reelCards;
    }
    void repopulateReel(); //Fill reelCards with random cards
    void iterateOnce(); //Effectively rotates the reel
};

