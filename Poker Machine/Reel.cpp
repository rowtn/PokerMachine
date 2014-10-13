#include "stdafx.h"
#include "Reel.h"

/* See Reel.h for function overviews */

Reel::Reel() {
    Reel::repopulateReel();
}


Reel::~Reel() { /* Do nothing*/ }

void Reel::repopulateReel() {
    for (int i = 15; i > 0; --i) { //Some say that -- is faster than ++
        reelCards.push_back(PokerCard::random()); //Uses statics to generate a new valid card
    }
}

void Reel::iterateOnce() {
    reelCards.insert(reelCards.begin(), reelCards.back()); //Makes the last element the first
    reelCards.pop_back(); //Removes the first element from list
}
