#pragma once

#include "PokerCard.h"
#include <vector>

class Reel {
private:
    std::vector<PokerCard> reelCards;
    int spins;
public:
    Reel(); //constructor
    ~Reel(); //destructor

    std::vector<PokerCard> getCards(void);
    void reInit(void);
    void repopulateReel(void); //Fill reelCards with random cards
    void iterateOnce(void); //Effectively rotates the reel (unless spins <= 0)
    int getSpinsLeft(void);
    void setSpins(int);
};

