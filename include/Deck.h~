#ifndef DECK_H
#define DECK_H

#include <iostream>
#include <vector>
using namespace std;

// Attributes of a card object:
// Rank : {2, 3, ..., 10, 11(J), 12(Q), 13(K), 14(A)}
// Suit : Club(1), Diamond(2), Heart(3), Spade(4)


class Card
{
    public:
        Card(int rank, int suit) : rank_(rank), suit_(suit) {};

        int rank() const { return rank_; };
        int suit() const { return suit_; };

        friend ostream& operator<<(ostream& os, const Card& card);

    private:
        int rank_;
        int suit_;

};

class Deck
{
    public:
        Deck();


    private:
        vector<Card> cards_;
};

#endif // DECK_H
