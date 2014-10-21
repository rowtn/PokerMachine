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
    spins = rand() % 10 + 10; //amount of spins to do after last spinner has finished
}

void Reel::iterateOnce() {
    if (spins <= 0) return; //if there are no remaining spins, do nothing
    reelCards.insert(reelCards.begin(), reelCards.back()); //Makes the last element the first
    reelCards.pop_back(); //Removes the first element from list
    spins--;
}

int Reel::getSpinsLeft() {
    return spins;
}

void Reel::setSpins(int s) {
    spins = s;
}

void Reel::reInit() {
    spins = rand() % 10 + 10; //add random amount of spins. same as repopulateReel, but keeps all cards in the reel
}

std::vector<PokerCard> Reel::getCards() {
    return reelCards;
}
