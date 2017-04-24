#ifndef DEALER_H
#define DEALER_H

#include "Player.h"


class Dealer
{
    public:
        Dealer() { deck_.shuffle(); };

        void deal_pockets(vector<Player> &players);
        void deal_flop();
        void deal_turn();
        void deal_river();

        Deck deck() const { return deck_; };
        vector<Card> community_cards() const { return community_cards_; };


        friend ostream& operator<<(ostream& os, const Dealer& dealer)
        {
            os << "\n\t\t";
            for(int i=0; i<dealer.community_cards().size(); i++)
                os << dealer.community_cards()[i];
            os << endl;
            return os;
        };

    private:
        Deck deck_;
        vector<Card> community_cards_;
};

#endif // DEALER_H
