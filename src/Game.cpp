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
        cout << "TBC.." << endl;
        //winner_id_ = //ShowHands.Winner(players_[0].pocket_cards(), players_[1].pocket_cards(), D.communitycards());
        if(winner_id_ != -1)
        {
            update_winner_stack();
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

Game::~Game()
{
    //dtor
}
