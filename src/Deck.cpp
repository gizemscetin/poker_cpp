#include "Deck.h"

string Ranks[14]    =  {"", "A", "2", "3", "4", "5", "6", "7", "8",
                        "9", "10", "J", "Q", "K"};
char Suits[4]     = {'C', 'D', 'H', 'S'}; //{'♣', '♦', '♥', '♠'};

ostream& operator<<(ostream& os, const Card& card)
{
    os << "[" << Ranks[card.rank()] << ' ' << Suits[card.suit()] << "]";
    return os;
};

vector<float> Card::get_card_info()
{
    vector<float> card_info;
    card_info.push_back(rank_);
    card_info.push_back(suit_);
    return card_info;
}

//////////////////////////////

Deck::Deck()
{
    //ctor
    for(int i=0; i<4; i++)
    {
        for(int j=1; j<14; j++)
        {
            Card C(j, i);
            cards_.push_back(C);
        }
    }
}


void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards_.begin(), cards_.end(), g);
}
Card Deck::pop()
{
    Card back = cards_.back();
    cards_.pop_back();
    return back;
}
void Deck::remove(vector<Card> cards)
{
    for(int i=0; i<cards.size(); i++)
    {
        cards_.erase(find(cards_.begin(), cards_.end(), cards[i]));
    }
}



ostream& operator<<(ostream& os, const Deck& deck)
{
    for(int i=0; i<deck.length(); i++)
    {
        os << deck.cards()[i] << " ";
    }
    return os;
}
