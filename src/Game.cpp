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
bool Game::play_one_state(vector<Card> community_cards, int state, bool io_on)
{
    int last_action = -1;
    int player_turn = 0;

    Player* P = players_[0];
    Player* P2 = players_[1];

    while(last_action != fold)
    {
        // MAKE THE FOLLOWING AUTOMATED
        P->act(community_cards, P2->action_history(), last_action, pot_, state);

        if(P->last_action() == -1) // No more turns left, so go to the next state
            break;

        if(io_on)
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
        P = players_[player_turn%2];
        P2 = players_[(player_turn+1)%2];

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
        if(players_[i]->check_id(winner_id_+1))
        {
            players_[i]->increase_stack(pot_);
            break;
        }
    }
}

void Game::play_one_round(bool io_on)
{
    Dealer D;				// Create new dealer for every round
    pot_ = 3;				// Clear the pot by setting it to small + big blinds

    winner_id_ = -1;        // -1 -> tie, 0 -> player 1, 1 -> player 2

    if(io_on)
        show_status();

    players_[0]->set_blind_type(0);
    players_[1]->set_blind_type(1);
    D.deal_pockets(players_);

    if(play_one_state(D.community_cards(), 0, io_on))   // Pre-flop
    {
        D.deal_flop();
        if(io_on)
            cout << D << endl;
        if(play_one_state(D.community_cards(), 1, io_on))   // Pre-turn
        {
            D.deal_turn();
            if(io_on)
                cout << D << endl;
            if(play_one_state(D.community_cards(), 1, io_on))   // Pre-river
            {
                D.deal_river();
                if(io_on)
                    cout << D << endl;
                play_one_state(D.community_cards(), 1, io_on);   // Post-river
                if(io_on)
                    cout << endl << endl;
            }
        }
    }

    // Find the winner of the round
    if(winner_id_ != -1)
    {
        if(io_on)
            cout << "Winner : Player " << winner_id_+1 << " -> " << pot_ << endl;
        update_winner_stack();
    }
    else
    {
        winner_id_ = find_winner(players_[0]->pocket_cards(), players_[1]->pocket_cards(), D.community_cards(), io_on);

        if(io_on)
        {
            show_player_pockets();
            show_status();
        }

        if(winner_id_ != -1)
        {
            if(io_on)
                cout << "Winner : Player " << players_[winner_id_]->id() << " -> " << pot_ << endl;
            players_[winner_id_]->increase_stack(pot_);
        }
        else // -1 -> tie
        {
            if(io_on)
                cout << "It's a tie. -> " << pot_ << endl;
            players_[0]->increase_stack(pot_/2);
            players_[1]->increase_stack(pot_/2);
        }
    }

    if(io_on)
        show_status();
}

void Game::start(bool io_on, int num_rounds)
{
    bool game_end = false;
	int	round_cnt = 0;

    while(round_cnt < num_rounds)
    {
        if(io_on)
        {
            cout << "\n----------------------------------------------------\n";
            cout << "\t\t***Round " << round_cnt+1 << "***" << endl;
        }

        play_one_round(io_on);

        if(io_on)
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

		// Switch players for the next round:
        reverse(players_.begin(), players_.end());
        round_cnt += 1;
    }

    if(io_on)
        show_status();
}

/*
int Game::find_winner(vector<Card> P1, vector<Card> P2, vector<Card> CommunityCards, bool io_on)
{
    HandRank H;

    vector<Card> pl1_best, pl2_best;
    int pl1_rank, pl2_rank;
    int result;

    pl1_best = H.RoyalFlush(P1, CommunityCards);
    pl1_rank = 9;
    if(pl1_best[0].rank() == 0)
    {
        pl1_best = H.StraightFlush(P1, CommunityCards);
        pl1_rank--;
        if(pl1_best[0].rank() == 0)
        {
            pl1_best = H.FourOfAKind(P1, CommunityCards);
            pl1_rank--;
            if(pl1_best[0].rank() == 0)
            {
                pl1_best = H.FullHouse(P1, CommunityCards);
                pl1_rank--;
                if(pl1_best[0].rank() == 0)
                {
                    pl1_best = H.Flush(P1, CommunityCards);
                    pl1_rank--;
                    if(pl1_best[0].rank() == 0)
                    {
                        pl1_best = H.Straight(P1, CommunityCards);
                        pl1_rank--;
                        if(pl1_best[0].rank() == 0)
                        {
                            pl1_best = H.ThreeOfAKind(P1, CommunityCards);
                            pl1_rank--;
                            if(pl1_best[0].rank() == 0)
                            {
                                pl1_best = H.TwoPair(P1, CommunityCards);
                                pl1_rank--;
                                if(pl1_best[0].rank() == 0)
                                {
                                    pl1_best = H.OnePair(P1, CommunityCards);
                                    pl1_rank--;
                                    if(pl1_best[0].rank() == 0)
                                    {
                                        pl1_best = H.HighCard(P1, CommunityCards);
                                        pl1_rank--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    pl2_best = H.RoyalFlush(P1, CommunityCards);
    pl2_rank = 9;
    if(pl2_best[0].rank() == 0)
    {
        pl2_best = H.StraightFlush(P1, CommunityCards);
        pl2_rank--;
        if(pl2_best[0].rank() == 0)
        {
            pl2_best = H.FourOfAKind(P1, CommunityCards);
            pl2_rank--;
            if(pl2_best[0].rank() == 0)
            {
                pl2_best = H.FullHouse(P1, CommunityCards);
                pl2_rank--;
                if(pl2_best[0].rank() == 0)
                {
                    pl2_best = H.Flush(P1, CommunityCards);
                    pl2_rank--;
                    if(pl2_best[0].rank() == 0)
                    {
                        pl2_best = H.Straight(P1, CommunityCards);
                        pl2_rank--;
                        if(pl2_best[0].rank() == 0)
                        {
                            pl2_best = H.ThreeOfAKind(P1, CommunityCards);
                            pl2_rank--;
                            if(pl2_best[0].rank() == 0)
                            {
                                pl2_best = H.TwoPair(P1, CommunityCards);
                                pl2_rank--;
                                if(pl2_best[0].rank() == 0)
                                {
                                    pl2_best = H.OnePair(P1, CommunityCards);
                                    pl2_rank--;
                                    if(pl2_best[0].rank() == 0)
                                    {
                                        pl2_best = H.HighCard(P1, CommunityCards);
                                        pl2_rank--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if(io_on)
    {
        for(int i=0; i<pl1_best.size(); i++)
        {
            cout << pl1_best[i];
        }
        cout << endl;
        for(int i=0; i<pl2_best.size(); i++)
        {
            cout << pl2_best[i];
        }
    }

    if(pl1_rank > pl2_rank)
    {
        result = 0;
        if(io_on)
            cout << "Player 1 won with a " << HandRankIOMap[pl1_rank] << endl;
    }
    else if(pl1_rank < pl2_rank)
    {
        result = 1;
        if(io_on)
            cout << "Player 2 won with a " << HandRankIOMap[pl2_rank] << endl;
    }
    else
    {
        if(pl1_rank == 7) //Four of a kind
        {

        }
    }
}
*/

int Game::find_winner(vector<Card> P1, vector<Card> P2, vector<Card> CommunityCards, bool io_on)
{
    if(io_on)
    {
        //cout << P1[0] << P1[1] << endl << P2[0] << P2[1] << endl;
        //cout << CommunityCards[0]<< CommunityCards[1]<< CommunityCards[2]<< CommunityCards[3]<< CommunityCards[4] << endl;
    }
    HandRank H;

    vector<Card> pl1_best, pl2_best;
    int result;

    pl1_best = H.RoyalFlush(P1, CommunityCards);
    pl2_best = H.RoyalFlush(P2, CommunityCards);

    if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
    {
        if(io_on)
        {
            cout << "Royal Flush" << endl;
            for(int i=0; i<pl1_best.size(); i++)
            {
                cout << pl1_best[i];
            }
            cout << endl;
            for(int i=0; i<pl2_best.size(); i++)
            {
                cout << pl2_best[i];
            }
        }
        if(pl1_best[0].rank() > pl2_best[0].rank())
        {
            result = 0;
            return result;
        }
        else if(pl1_best[0].rank() < pl2_best[0].rank())
        {
            result = 1;
            return result;
        }
        else
        {
            result = -1;
            return result;
        }
    }
    else
    {
        pl1_best = H.StraightFlush(P1, CommunityCards);
        pl2_best = H.StraightFlush(P2, CommunityCards);

        if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
        {
            if(io_on)
            {
                cout << "Straight Flush" << endl;
                for(int i=0; i<pl1_best.size(); i++)
                {
                    cout << pl1_best[i];
                }
                cout << endl;
                for(int i=0; i<pl2_best.size(); i++)
                {
                    cout << pl2_best[i];
                }
                cout << endl;
            }
            if(pl1_best[0].rank() > pl2_best[0].rank())
            {
                result = 0;
                return result;
            }
            else if(pl1_best[0].rank() < pl2_best[0].rank())
            {
                result = 1;
                return result;
            }
            else
            {
                result = -1;
                return result;
            }
        }

        else
        {
            pl1_best = H.FourOfAKind(P1, CommunityCards);
            pl2_best = H.FourOfAKind(P2, CommunityCards);

            if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
            {
                if(io_on)
                {
                    cout << "Four Of A Kind" << endl;
                    for(int i=0; i<pl1_best.size(); i++)
                    {
                        cout << pl1_best[i];
                    }
                    cout << endl;
                    for(int i=0; i<pl2_best.size(); i++)
                    {
                        cout << pl2_best[i];
                    }
                    cout << endl;
                }
                if(pl1_best[0].rank() > pl2_best[0].rank())
                {
                    result = 0;
                    return result;
                }
                else if(pl1_best[0].rank() < pl2_best[0].rank())
                {
                    result = 1;
                    return result;
                }
                else
                {
                    if(pl1_best[4].rank() > pl2_best[4].rank())
                    {
                        result = 0;
                    }
                    else if(pl1_best[4].rank() < pl2_best[4].rank())
                    {
                        result = 1;
                    }
                    else
                    {
                        result = -1;
                        return result;
                    }
                }
            }
            else
            {
                pl1_best = H.FullHouse(P1, CommunityCards);
                pl2_best = H.FullHouse(P2, CommunityCards);

                if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
                {
                    if(io_on)
                    {
                        cout << "Full House" << endl;
                        for(int i=0; i<pl1_best.size(); i++)
                        {
                            cout << pl1_best[i];
                        }
                        cout << endl;
                        for(int i=0; i<pl2_best.size(); i++)
                        {
                            cout << pl2_best[i];
                        }
                        cout << endl;
                    }
                    if(pl1_best[0].rank() > pl2_best[0].rank())
                    {
                        result = 0;
                        return result;
                    }
                    else if(pl1_best[0].rank() < pl2_best[0].rank())
                    {
                        result = 1;
                        return result;
                    }
                    else
                    {
                        result = -1;
                        return result;
                    }
                }
                else
                {
                    pl1_best = H.Flush(P1, CommunityCards);
                    pl2_best = H.Flush(P2, CommunityCards);

                    if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
                    {
                        if(io_on)
                        {
                            cout << "Flush" << endl;
                            for(int i=0; i<pl1_best.size(); i++)
                            {
                                cout << pl1_best[i];
                            }
                            cout << endl;
                            for(int i=0; i<pl2_best.size(); i++)
                            {
                                cout << pl2_best[i];
                            }
                            cout << endl;
                        }
                        if(pl1_best[0].rank() > pl2_best[0].rank())
                        {
                            result = 0;
                            return result;
                        }
                        else if(pl1_best[0].rank() < pl2_best[0].rank())
                        {
                            result = 1;
                            return result;
                        }
                        else
                        {
                            result = -1;
                            return result;
                        }
                    }
                    else
                    {
                        pl1_best = H.Straight(P1, CommunityCards);
                        pl2_best = H.Straight(P2, CommunityCards);

                        if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
                        {
                            if(io_on)
                            {
                                cout << "Straight" << endl;
                                for(int i=0; i<pl1_best.size(); i++)
                                {
                                    cout << pl1_best[i];
                                }
                                cout << endl;
                                for(int i=0; i<pl2_best.size(); i++)
                                {
                                    cout << pl2_best[i];
                                }
                                cout << endl;
                            }
                            if(pl1_best[0].rank() > pl2_best[0].rank())
                            {
                                result = 0;
                                return result;
                            }
                            else if(pl1_best[0].rank() < pl2_best[0].rank())
                            {
                                result = 1;
                                return result;
                            }
                            else
                            {
                                result = -1;
                                return result;
                            }
                        }
                        else
                        {
                            pl1_best = H.ThreeOfAKind(P1, CommunityCards);
                            pl2_best = H.ThreeOfAKind(P2, CommunityCards);

                            if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
                            {
                                if(io_on)
                                {
                                    cout << "Three of a kind" << endl;
                                    for(int i=0; i<pl1_best.size(); i++)
                                    {
                                        cout << pl1_best[i];
                                    }
                                    cout << endl;
                                    for(int i=0; i<pl2_best.size(); i++)
                                    {
                                        cout << pl2_best[i];
                                    }
                                    cout << endl;
                                }
                                if(pl1_best[0].rank() > pl2_best[0].rank())
                                {
                                    result = 0;
                                    return result;
                                }
                                else if(pl1_best[0].rank() < pl2_best[0].rank())
                                {
                                    result = 1;
                                    return result;
                                }
                                else // Check Kicker
                                {
                                    if(pl1_best[3].rank() > pl2_best[3].rank())
                                    {
                                        result = 0;
                                        return result;
                                    }
                                    else if(pl1_best[3].rank() < pl2_best[3].rank())
                                    {
                                        result = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        if(pl1_best[4].rank() > pl2_best[4].rank())
                                        {
                                            result = 0;
                                            return result;
                                        }
                                        else if(pl1_best[4].rank() < pl2_best[4].rank())
                                        {
                                            result = 1;
                                            return result;
                                        }
                                        else
                                            result = -1;
                                            return result;
                                        }
                                }
                            }
                            else
                            {
                                pl1_best = H.TwoPair(P1, CommunityCards);
                                pl2_best = H.TwoPair(P2, CommunityCards);

                                if((pl1_best[0].rank() != 0 && pl1_best[2].rank() != 0) ||
                                   (pl2_best[0].rank() != 0 && pl2_best[2].rank() != 0))
                                {
                                    if(io_on)
                                    {
                                        cout << "Two Pairs" << endl;
                                        for(int i=0; i<pl1_best.size(); i++)
                                        {
                                            cout << pl1_best[i];
                                        }
                                        cout << endl;
                                        for(int i=0; i<pl2_best.size(); i++)
                                        {
                                            cout << pl2_best[i];
                                        }
                                        cout << endl;
                                    }
                                    if(pl1_best[0].rank() > pl2_best[0].rank())
                                    {
                                        result = 0;
                                        return result;
                                    }
                                    else if(pl1_best[0].rank() < pl2_best[0].rank())
                                    {
                                        result = 1;
                                        return result;
                                    }
                                    else // Check Kicker
                                    {
                                        if(pl1_best[2].rank() > pl2_best[2].rank())
                                        {
                                            result = 0;
                                            return result;
                                        }
                                        else if(pl1_best[2].rank() < pl2_best[2].rank())
                                        {
                                            result = 1;
                                            return result;
                                        }
                                        else // Check Kicker
                                        {
                                            if(pl1_best[4].rank() > pl2_best[4].rank())
                                            {
                                                result = 0;
                                                return result;
                                            }
                                            else if(pl1_best[4].rank() < pl2_best[4].rank())
                                            {
                                                result = 1;
                                                return result;
                                            }
                                            else
                                            {
                                                result = -1;
                                                return result;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    pl1_best = H.OnePair(P1, CommunityCards);
                                    pl2_best = H.OnePair(P2, CommunityCards);

                                    if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
                                    {
                                        if(io_on)
                                        {
                                            cout << "One Pair" << endl;
                                            for(int i=0; i<pl1_best.size(); i++)
                                            {
                                                cout << pl1_best[i];
                                            }
                                            cout << endl;
                                            for(int i=0; i<pl2_best.size(); i++)
                                            {
                                                cout << pl2_best[i];
                                            }
                                            cout << endl;
                                        }
                                        if(pl1_best[0].rank() > pl2_best[0].rank())
                                        {
                                            result = 0;
                                            return result;
                                        }
                                        else if(pl1_best[0].rank() < pl2_best[0].rank())
                                        {
                                            result = 1;
                                            return result;
                                        }
                                        else // Check Kicker
                                        {
                                            if(pl1_best[2].rank() > pl2_best[2].rank())
                                            {
                                                result = 0;
                                                return result;
                                            }
                                            else if(pl1_best[2].rank() < pl2_best[2].rank())
                                            {
                                                result = 1;
                                                return result;
                                            }
                                            else
                                            {
                                                if(pl1_best[3].rank() > pl2_best[3].rank())
                                                {
                                                    result = 0;
                                                    return result;
                                                }
                                                else if(pl1_best[3].rank() < pl2_best[3].rank())
                                                {
                                                    result = 1;
                                                    return result;
                                                }
                                                else
                                                {
                                                    if(pl1_best[4].rank() > pl2_best[4].rank())
                                                    {
                                                        result = 0;
                                                        return result;
                                                    }
                                                    else if(pl1_best[4].rank() < pl2_best[4].rank())
                                                    {
                                                        result = 1;
                                                        return result;
                                                    }
                                                    else
                                                    {
                                                        result = -1;
                                                        return result;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        pl1_best = H.HighCard(P1, CommunityCards);
                                        pl2_best = H.HighCard(P2, CommunityCards);
                                        //if(pl1_best[0].rank() != 0 || pl2_best[0].rank() != 0)
                                        //{
                                            if(io_on)
                                            {
                                                cout << "High Card" << endl;
                                                for(int i=0; i<pl1_best.size(); i++)
                                                {
                                                    cout << pl1_best[i];
                                                }
                                                cout << endl;
                                                for(int i=0; i<pl2_best.size(); i++)
                                                {
                                                    cout << pl2_best[i];
                                                }
                                                cout << endl;
                                            }
                                            if(pl1_best[0].rank() > pl2_best[0].rank())
                                            {
                                                result = 0;
                                                return result;
                                            }
                                            else if(pl1_best[0].rank() < pl2_best[0].rank())
                                            {
                                                result = 1;
                                                return result;
                                            }
                                            else
                                            {
                                                if(pl1_best[1].rank() > pl2_best[1].rank())
                                                {
                                                    result = 0;
                                                    return result;
                                                }
                                                else if(pl1_best[1].rank() < pl2_best[1].rank())
                                                {
                                                    result = 1;
                                                    return result;
                                                }
                                                else
                                                {
                                                    if(pl1_best[2].rank() > pl2_best[2].rank())
                                                    {
                                                        result = 0;
                                                        return result;
                                                    }
                                                    else if(pl1_best[2].rank() < pl2_best[2].rank())
                                                    {
                                                        result = 1;
                                                        return result;
                                                    }
                                                    else
                                                    {
                                                        if(pl1_best[3].rank() > pl2_best[3].rank())
                                                        {
                                                            result = 0;
                                                            return result;
                                                        }
                                                        else if(pl1_best[3].rank() < pl2_best[3].rank())
                                                        {
                                                            result = 1;
                                                            return result;
                                                        }
                                                        else
                                                        {
                                                            if(pl1_best[4].rank() > pl2_best[4].rank())
                                                            {
                                                                result = 0;
                                                                return result;
                                                            }
                                                            else if(pl1_best[4].rank() < pl2_best[4].rank())
                                                            {
                                                                result = 1;
                                                                return result;
                                                            }
                                                            else
                                                            {
                                                                result = -1;
                                                                return result;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        //}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

