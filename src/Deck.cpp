#include "Deck.h"

string Ranks[15]    =  {"0", "A", "2", "3", "4", "5", "6", "7", "8",
                        "9", "10", "J", "Q", "K", "A"};
char32_t Suits[5]     =  {0, U'♣', U'♦', U'♥', U'♠'};

Deck::Deck()
{
    //ctor
}


ostream& operator<<(ostream& os, const Card& card)
{
    os << Ranks[card.rank()] << ' ' << Suits[card.suit()];
    return os;
}
