#include "Game.h"

Game::Game(int player_count)
{
    // Create players
    for(int i=1; i<=player_count; i++)
    {
        players_.push_back(Player(i));
    }
}

void Game::show_status()
{
    cout << *this << endl;
    for(int i=0; i<players_.size(); i++)
    {
        cout << players_[i] << endl;
    }
}

void Game::show_player_pockets()
{
    for(int i=0; i<players_.size(); i++)
    {
        cout << "Player " << players_[i].id() << " : ";
        players_[i].show_pockets();
    }
}

bool Game::play_one_state(int state)
{
    int last_action = -1;
    int player_turn = 0;
    Player* P = &players_[0];

    while(last_action != fold)
    {
        P->act(state, last_action);

        if(P->last_action() == -1) // No more turns left, so go to the next state
            break;
        P->print_last_action();


        // Update the new last action
		last_action = P->last_action();
		P->add_action(last_action);

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
        player_turn++;
        P = &players_[player_turn%2];

        if(last_action == fold)
        {
            winner_id_ = P->id() - 1;    // 0 -> player 1, 1 -> player 2
            return false;
        }
    }
    return true;
}

void Game::update_winner_stack()
{
    for(int i=0; i<players_.size(); i++)
    {
        if(players_[i].check_id(winner_id_+1))
        {
            players_[i].increase_stack(pot_);
            break;
        }
    }
}

void Game::play_one_round()
{
    Dealer D;				// Create new dealer for every round
    pot_ = 3;				// Clear the pot by setting it to small + big blinds

    winner_id_ = -1;        // -1 -> tie, 0 -> player 1, 1 -> player 2

    players_[0].set_blind_type(0);
    players_[1].set_blind_type(1);

    show_status();

    D.deal_pockets(players_);
    show_player_pockets();

    if(play_one_state(0))   // Pre-flop
    {
        D.deal_flop();
        cout << D << endl;
        show_status();

        if(play_one_state(1))   // Pre-turn
        {
            D.deal_turn();
            cout << D << endl;
            show_status();

            if(play_one_state(1))   // Pre-river
            {
                D.deal_river();
                cout << D << endl;
            }
        }
    }

    // Find the winner of the round
    if(winner_id_ != -1)
    {
        update_winner_stack();
    }
    else
    {
        // Use winner function!!!
        //cout << "TBC.." << endl;
        winner_id_ = find_winner(players_[0].pocket_cards(), players_[1].pocket_cards(), D.community_cards());
        if(winner_id_ != -1)
        {
            //update_winner_stack();
            players_[winner_id_].increase_stack(pot_);
        }
        else // -1 -> tie
        {
            players_[0].increase_stack(pot_/2);
            players_[1].increase_stack(pot_/2);
        }
    }

    show_status();
}

void Game::start()
{
    bool game_end = false;
	int	round_cnt = 0;

    while(round_cnt < 3 && !game_end)
    {
        cout << "\n----------------------------------\n";
        play_one_round();
        cout << "\n----------------------------------\n";

        // Before going into the next round, do a few things!
		//Check game over conditions:
        for(int i=0; i<players_.size(); i++)
        {
            if(players_[i].lost())
                game_end = true;
        }

        // Clear cards for the next round:

		// Switch players for the next round:
        reverse(players_.begin(), players_.end());
        round_cnt += 1;
    }
}

int Game::find_winner(vector<Card> P1, vector<Card> P2, vector<Card> CommunityCards)
{
    HandRank H;

	if ((H.RoyalFlush(P1, CommunityCards))[0].rank() != 0 || (H.RoyalFlush(P2, CommunityCards))[0].rank() != 0) {
		for (int n = 0; n < 5; n++) {
			if ((H.RoyalFlush(P1, CommunityCards))[n].rank() > (H.RoyalFlush(P2, CommunityCards))[n].rank()) {
				return 0; //Player 1 is the Winner (Royal Flush)
			}
			if ((H.RoyalFlush(P1, CommunityCards))[n].rank() < (H.RoyalFlush(P2, CommunityCards))[n].rank()) {
				return 1; //Player 2 is the Winner (Royal Flush)
			}
		}
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
		return -1; //It's a Tie (Straight Flush)
	}
	else if (((H.FourOfAKind(P1, CommunityCards))[0].rank() != 0) || (H.FourOfAKind(P2, CommunityCards))[0].rank() != 0) {
		for (int n = 0; n < 5; n++) {
			if ((H.FourOfAKind(P1, CommunityCards))[n].rank() > (H.FourOfAKind(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Four of a Kind)
			if ((H.FourOfAKind(P1, CommunityCards))[n].rank() < (H.FourOfAKind(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Four of a Kind)
		}
		return -1; //It's a Tie (Four of a Kind)
	}
	else if (((H.FullHouse(P1, CommunityCards))[0].rank() != 0) || ((H.FullHouse(P2, CommunityCards))[0].rank() != 0)) {
		for (int n = 0; n < 5; n++) {
			if ((H.FullHouse(P1, CommunityCards))[n].rank() > (H.FullHouse(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Full House)
			if ((H.FullHouse(P1, CommunityCards))[n].rank() < (H.FullHouse(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Full House)
		}
		return -1; //It's a Tie (Full House)
	}
	else if (((H.Flush(P1, CommunityCards))[0].rank() != 0) || ((H.Flush(P2, CommunityCards))[0].rank() != 0)) {
		for (int n = 0; n < 5; n++) {
			if ((H.Flush(P1, CommunityCards))[n].rank() > (H.Flush(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Flush)
			if ((H.Flush(P1, CommunityCards))[n].rank() < (H.Flush(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Flush)
		}
		return -1; //It's a Tie (Flush)
	}
	else if (((H.Straight(P1, CommunityCards))[0].rank() != 0) || ((H.Straight(P2, CommunityCards))[0].rank() != 0)) {
		for (int n = 0; n < 5; n++) {
			if ((H.Straight(P1, CommunityCards))[n].rank() > (H.Straight(P2, CommunityCards))[n].rank())
				return 0; //Player 1 is the Winner (Straight)
			if ((H.Straight(P1, CommunityCards))[n].rank() < (H.Straight(P2, CommunityCards))[n].rank())
				return 1; //Player 2 is the Winner (Straight)
		}
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
			return -1; //It's a Tie (Two Pair)
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
		return -1; //It's a Tie (High Card)
	}
}
