#include "Dealer.h"

void Dealer::deal_pockets(vector<Player*> players)
{

    // First Card
    for(int i=0; i<players.size(); i++)
    {
        players[i]->clear_cards();
        players[i]->add_card(deck_.pop());
    }

    // Second Card
    for(int i=0; i<players.size(); i++)
    {
        players[i]->add_card(deck_.pop());
    }
}
void Dealer::deal_flop()
{
    // Burn one card
	deck_.pop();
	// Flop (three community cards)
	community_cards_.push_back(deck_.pop());
	community_cards_.push_back(deck_.pop());
	community_cards_.push_back(deck_.pop());
}
void Dealer::deal_turn()
{
    // Burn one card
	deck_.pop();
	// Turn (one community cards)
	community_cards_.push_back(deck_.pop());
}
void Dealer::deal_river()
{
    // Burn one card
	deck_.pop();
	// River (one community cards)
	community_cards_.push_back(deck_.pop());
}
