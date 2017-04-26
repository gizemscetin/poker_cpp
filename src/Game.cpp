#include "Game.h"

Game::Game(Player *pl1, Player *pl2)
//(int player_count)
{
    // Create players
    //cout << "Constructor 1" << endl;
    pl1->set_id(1);
    pl2->set_id(2);

    players_.push_back(pl1);
    players_.push_back(pl2);
    //cout << "Constructor 2" << endl;
}

void Game::show_status()
{
    //cout << *this << endl;
    for(int i=0; i<players_.size(); i++)
    {
        cout << *players_[i] << endl;
    }
}

void Game::show_player_pockets()
{
    for(int i=0; i<players_.size(); i++)
    {
        cout << "Player " << players_[i]->id() << " : ";
        players_[i]->show_pockets();
    }
}

/*
void Game::player_act(OrganicNNet::NNET *network, int state)
{
    vector<float> input1 = players_[0]->pocket_cards()[0].get_card_info();
    vector<float> input2 = players_[0]->pocket_cards()[1].get_card_info();
    vector<float> input3;
    input3.push_back(players_[0]->blind_type());


}
*/
bool Game::play_one_state(vector<Card> community_cards, int state)
{
    int last_action = -1;
    int player_turn = 0;

    Player* P = players_[0];
    Player* P2 = players_[1];

    while(last_action != fold)
    {
        // MAKE THE FOLLOWING AUTOMATED
        //P->act(state, last_action);
        try
        {
            P->act(community_cards, P2->action_history(), last_action, pot_, state);
        }
        catch(exception e)
        {
            cout << "Problem in Player.act()" << endl;
        }

        if(P->last_action() == -1) // No more turns left, so go to the next state
            break;

        P->print_last_action();


        // Update the new last action
		last_action = P->last_action();
		P->add_action(last_action);
		//P->print_action_history();


		// Update the current stack according to the action
        pot_ += P->decrease_stack_wrt_action(last_action);

        // Handle 2 special cases here!!!
		// Call -> Check and Raise -> Bet
        if(player_turn == 0)
        {
			if(last_action == 1)
				last_action = 0;
			else if(last_action == 3)
                last_action = 2;
        }


        // Update the counter
        try
        {
            player_turn++;
            P = players_[player_turn%2];
            P2 = players_[(player_turn+1)%2];

            if(last_action == fold)
            {
                winner_id_ = P->id() - 1;    // 0 -> player 1, 1 -> player 2
                return false;
            }
        }
        catch(exception &e)
        {
            cout << "weird." << endl;
        }
    }
    return true;
}

void Game::update_winner_stack()
{
    for(int i=0; i<players_.size(); i++)
    {
        if(players_[i]->check_id(winner_id_+1))
        {
            players_[i]->increase_stack(pot_);
            break;
        }
    }
}

void Game::play_one_round()
{
    Dealer D;				// Create new dealer for every round
    pot_ = 3;				// Clear the pot by setting it to small + big blinds

    winner_id_ = -1;        // -1 -> tie, 0 -> player 1, 1 -> player 2

    show_status();
    //cout << "A" << endl;
    players_[0]->set_blind_type(0);
    players_[1]->set_blind_type(1);
    //cout << "B" << endl;

    //show_status();


    D.deal_pockets(players_);
    //show_player_pockets();

    try{
        if(play_one_state(D.community_cards(), 0))   // Pre-flop
        {
            D.deal_flop();
            //cout << D << endl;
            //show_status();

            if(play_one_state(D.community_cards(), 1))   // Pre-turn
            {
                D.deal_turn();
                //cout << D << endl;
                //show_status();

                if(play_one_state(D.community_cards(), 1))   // Pre-river
                {
                    D.deal_river();
                    //cout << D << endl;
                }
            }
        }
    }
    catch(exception &e)
    {
        cout << "Problem in play one state()" << endl;
    }

    // Find the winner of the round
    try{
        if(winner_id_ != -1)
        {
            update_winner_stack();
            cout << "Winner : Player " << winner_id_ << " -> " << pot_ << endl;
        }
        else
        {
            // Use winner function!!!
            //cout << "TBC.." << endl;
            winner_id_ = find_winner(players_[0]->pocket_cards(), players_[1]->pocket_cards(), D.community_cards());

            if(winner_id_ != -1)
            {
                cout << "Winner : Player " << players_[winner_id_]->id() << " -> " << pot_ << endl;
                //update_winner_stack();
                //cout << "Increasing stack. " << endl;
                players_[winner_id_]->increase_stack(pot_);
                //cout << "Increased stack. " << endl;
            }
            else // -1 -> tie
            {
                cout << "It's a tie. -> " << pot_ << endl;
                //cout << "Splitting stack. " << endl;
                players_[0]->increase_stack(pot_/2);
                players_[1]->increase_stack(pot_/2);
                //cout << "Splitted stack. " << endl;
            }
        }
    }
    catch(exception &e)
    {
        cout << "Problem in finding the winner. " << endl;
    }

    //show_status();
}

void Game::start()
{
    bool game_end = false;
	int	round_cnt = 0;

    while(round_cnt < 3)
    {
        cout << "\n----------------------------------------------------\n";
        cout << "\t\t***Round " << round_cnt+1 << "***" << endl;
        try
        {
            play_one_round();
            //cout << "No problem." << endl;
        }
        catch(exception &e)
        {
            cout << "Problem in  start()." << endl;
        }
        cout << "\n----------------------------------------------------\n";

        // Before going into the next round, do a few things!
		//Check game over conditions:
        /*
        for(int i=0; i<players_.size(); i++)
        {
            if(players_[i]->lost())
                game_end = true;
        }
        */
        // Clear cards for the next round:

		// Switch players for the next round:
        reverse(players_.begin(), players_.end());
        //cout << "Reverse success. " << endl;
        round_cnt += 1;
    }
    show_status();
}

int Game::find_winner(vector<Card> P1, vector<Card> P2, vector<Card> CommunityCards)
{
    HandRank H;
    //cout << "Start find_winner " << endl;
	if ((H.RoyalFlush(P1, CommunityCards))[0].rank() != 0 || (H.RoyalFlush(P2, CommunityCards))[0].rank() != 0) {
		for (int n = 0; n < 5; n++) {
			if ((H.RoyalFlush(P1, CommunityCards))[n].rank() > (H.RoyalFlush(P2, CommunityCards))[n].rank()) {
				return 0; //Player 1 is the Winner (Royal Flush)
			}
			if ((H.RoyalFlush(P1, CommunityCards))[n].rank() < (H.RoyalFlush(P2, CommunityCards))[n].rank()) {
				return 1; //Player 2 is the Winner (Royal Flush)
			}
		}
		//cout << "End find_winner()" << endl;
		return -1; //It's a Tie (Royal Flush)
	}
	else if (((H.StraightFlush(P1, CommunityCards))[0].rank() != 0) || ((H.StraightFlush(P2, CommunityCards))[0].rank() != 0)) {
		for (int n = 0; n < 5; n++) {
			if ((H.StraightFlush(P1, CommunityCards))[n].rank() > (H.StraightFlush(P2, CommunityCards))[n].rank()) {
				return 0; //Player 1 is the Winner (Straight Flush)
			}
			if ((H.StraightFlush(P1, CommunityCards))[n].rank() < (H.StraightFlush(P2, CommunityCards))[n].rank()) {
				return 1; //Player 2 is the Winner (Straight Flush)
			}
		}
		//cout << "End find_winner()" << endl;
		return -1; //It's a Tie (Straight Flush)
	}
	else if (((H.FourOfAKind(P1, CommunityCards))[0].rank() != 0) || (H.FourOfAKind(P2, CommunityCards))[0].rank() != 0) {
		for (int n = 0; n < 5; n++) {
			if ((H.FourOfAKind(P1, CommunityCards))[n].rank() > (H.FourOfAKind(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Four of a Kind)
			if ((H.FourOfAKind(P1, CommunityCards))[n].rank() < (H.FourOfAKind(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Four of a Kind)
		}
		//cout << "End find_winner()" << endl;
		return -1; //It's a Tie (Four of a Kind)
	}
	else if (((H.FullHouse(P1, CommunityCards))[0].rank() != 0) || ((H.FullHouse(P2, CommunityCards))[0].rank() != 0)) {
		for (int n = 0; n < 5; n++) {
			if ((H.FullHouse(P1, CommunityCards))[n].rank() > (H.FullHouse(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Full House)
			if ((H.FullHouse(P1, CommunityCards))[n].rank() < (H.FullHouse(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Full House)
		}
		//cout << "End find_winner()" << endl;
		return -1; //It's a Tie (Full House)
	}
	else if (((H.Flush(P1, CommunityCards))[0].rank() != 0) || ((H.Flush(P2, CommunityCards))[0].rank() != 0)) {
		for (int n = 0; n < 5; n++) {
			if ((H.Flush(P1, CommunityCards))[n].rank() > (H.Flush(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Flush)
			if ((H.Flush(P1, CommunityCards))[n].rank() < (H.Flush(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Flush)
		}
		//cout << "End find_winner()" << endl;
		return -1; //It's a Tie (Flush)
	}
	else if (((H.Straight(P1, CommunityCards))[0].rank() != 0) || ((H.Straight(P2, CommunityCards))[0].rank() != 0)) {
		for (int n = 0; n < 5; n++) {
			if ((H.Straight(P1, CommunityCards))[n].rank() > (H.Straight(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Straight)
			if ((H.Straight(P1, CommunityCards))[n].rank() < (H.Straight(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Straight)
		}
		//cout << "End find_winner()" << endl;
		return -1; //It's a Tie (Straight)
	}
	else if (((H.ThreeOfAKind(P1, CommunityCards))[0].rank() != 0) || ((H.ThreeOfAKind(P2, CommunityCards))[0].rank() != 0)) {
		if ((H.ThreeOfAKind(P1, CommunityCards))[0].rank() > (H.ThreeOfAKind(P2, CommunityCards))[0].rank())
			return 0; //Player 1 is the Winner (Three of a Kind)
		else if ((H.ThreeOfAKind(P1, CommunityCards))[0].rank() < (H.ThreeOfAKind(P2, CommunityCards))[0].rank())
			return 1; //Player 2 is the Winner (Three of a Kind)
		else {
			for (int n = 0; n < 2; n++) {
				if ((H.ThreeOfAKind(P1, CommunityCards))[3 + n].rank() > (H.ThreeOfAKind(P2, CommunityCards))[3 + n].rank())
					return 0; //Player 1 is the Winner (Three of a Kind High Kicker)
				if ((H.ThreeOfAKind(P1, CommunityCards))[3 + n].rank() < (H.ThreeOfAKind(P2, CommunityCards))[3 + n].rank())
					return 1; //Player 2 is the Winner (Three of a Kind High Kicker)
			}
			//cout << "End find_winner()" << endl;
			return -1; //It's a Tie (Three of a Kind)
		}
	}
	else if ((((H.TwoPair(P1, CommunityCards))[0].rank() != 0) && ((H.TwoPair(P1, CommunityCards))[2].rank() != 0)) || (((H.TwoPair(P2, CommunityCards)[0].rank() != 0)) && (H.TwoPair(P2, CommunityCards)[2].rank() != 0))) {
		for (int n = 0; n < 4; n++) {
			if ((H.TwoPair(P1, CommunityCards))[n].rank() > (H.TwoPair(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Two Pair)
			if ((H.TwoPair(P1, CommunityCards))[n].rank() < (H.TwoPair(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Two Pair)
		}
		if ((H.TwoPair(P1, CommunityCards))[4].rank() > (H.TwoPair(P2, CommunityCards))[4].rank())
			return 0; //Player 1 is the Winner (Two Pair High Kicker)
		else if ((H.TwoPair(P1, CommunityCards))[4].rank() < (H.TwoPair(P2, CommunityCards))[4].rank())
			return 1; //Player 2 is the Winner (Two Pair High Kicker)
		else
        {
            //cout << "End find_winner()" << endl;
			return -1; //It's a Tie (Two Pair)
        }
	}
	else if (((H.OnePair(P1, CommunityCards))[0].rank() != 0) || ((H.OnePair(P2, CommunityCards))[0].rank() != 0)) {
		if ((H.OnePair(P1, CommunityCards))[0].rank() > (H.OnePair(P2, CommunityCards))[0].rank())
			return 0; //Player 1 is the Winner (One Pair)
		else if ((H.OnePair(P1, CommunityCards))[0].rank() < (H.OnePair(P2, CommunityCards))[0].rank())
			return 1; //Player 2 is the Winner (One Pair)
		else {
			for (int n = 0; n < 5; n++) {
				if ((H.HighCard(P1, CommunityCards))[(n)].rank() > (H.HighCard(P2, CommunityCards))[(n)].rank())
					return 0; //Player 1 is the Winner (One Pair High Kicker)
				if ((H.HighCard(P1, CommunityCards))[(n)].rank() < (H.HighCard(P2, CommunityCards))[(n)].rank())
					return 1; //Player 2 is the Winner (One Pair High Kicker)
			}
			//cout << "End find_winner()" << endl;
			return -1; //It's a Tie (One Pair)
		}
	}
	else {
		for (int n = 0; n < 5; n++) {
			if ((H.HighCard(P1, CommunityCards))[(n)].rank() > (H.HighCard(P2, CommunityCards))[(n)].rank())
				return 0; //Player 1 is the Winner (High Card)
			if ((H.HighCard(P1, CommunityCards))[(n)].rank() < (H.HighCard(P2, CommunityCards))[(n)].rank())
				return 1; //Player 2 is the Winner (High Card)
		}
		//cout << "End find_winner()" << endl;
		return -1; //It's a Tie (High Card)
	}
}
