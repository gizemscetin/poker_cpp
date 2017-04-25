#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "Deck.h"

using namespace std;

class HandRank {
public:
	vector<Card> RoyalFlush(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> StraightFlush(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> FourOfAKind(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> FullHouse(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> Flush(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> Straight(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> ThreeOfAKind(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> TwoPair(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> OnePair(vector<Card> player, vector<Card> CommunityCards);
	vector<Card> HighCard(vector<Card> player, vector<Card> CommunityCards);

    //private:
    //    vector<Card> cards_; // total 7 cards
};
