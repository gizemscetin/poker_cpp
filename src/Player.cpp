#include "Player.h"

string ActionIOMap[] = {"check", "call", "bet", "raise", "fold"};

void Player::show_pockets()
{
    if(pocket_cards_[0] < pocket_cards_[1])
        cout << pocket_cards_[0] << pocket_cards_[1] << endl;
    else
        cout << pocket_cards_[1] << pocket_cards_[0] << endl;
}

void Player::print_last_action()
{
		cout << "Player " << id_ << " ";
        cout << ActionIOMap[last_action_] << "s." << endl;
}

void Player::print_possible_actions(vector<int> actions)
{
    cout << "{ ";
    for(int i=0; i<actions.size(); i++)
        cout << ActionIOMap[actions[i]] << "-" << actions[i] << " ";
    cout << "} " << endl;
}

void Player::print_action_history()
{
    cout << "Player " << id_ << "'s moves: ";
    cout << "{ ";
    for(int i=0; i<action_history_.size(); i++)
        cout << ActionIOMap[action_history_[i]] << " ";
    cout << "} " << endl;
}

void Player::print_opponent_history()
{
    cout << "Player " << (3-id_) << "'s moves: ";
    cout << "{ ";
    for(int i=0; i<opponent_history_.size(); i++)
        cout << ActionIOMap[opponent_history_[i]] << " ";
    cout << "} " << endl;
}

void Player::print_stack_history()
{
    cout << "{ ";
    for(int i=0; i<stack_history_.size(); i++)
        cout << stack_history_[i] << " ";
    cout << "} ";
}

vector<int> Player::find_possible_actions(int state, int last_action)
{
    //Action Set : {check-0, call-1, bet-2, raise-3, fold-4}
    vector<int> action_set;

    if(last_action == -1)
    {
        if(state == 0) // Pre-flop
        {
            action_set.push_back(call);
            action_set.push_back(Raise);
            action_set.push_back(fold);
            return action_set;
        }
        action_set.push_back(check);
        action_set.push_back(bet);
        action_set.push_back(fold);
        return action_set;
    }

	if(blind_type_ != 0 && last_action == 0)
	{
        action_set.push_back(check);
        action_set.push_back(bet);
        action_set.push_back(fold);
        return action_set;
	}
    if(last_action == 2)
    {
        {
            action_set.push_back(call);
            action_set.push_back(Raise);
            action_set.push_back(fold);
            return action_set;
        }
    }

    if(state != 0 && last_action == 3)
    {
        action_set.push_back(call);
        action_set.push_back(fold);
        return action_set;
    }

	//	# For the pre-flop, if the last action was Raise (3):
	//	# the possible action sets can be: S1, S3
	//	# Handle S1 case by changing that first Raise to Bet (2)
    if(last_action == 3)
    {
        action_set.push_back(call);
        action_set.push_back(fold);
        return action_set;
    }

    return action_set;
}

int Player::compute_total_win()
{
    int total = 0;
    for(int i=0; i<stack_history_.size(); i++)
        total += (stack_history_[i] - 1000);
    return total;
}

void Player::act(vector<Card> community_cards, vector<int> opponent_history,
                 int last_action, int current_pot, int state)//(int state, int last_action)
{
    //Action Set : {check-0, call-1, bet-2, raise-3, fold-4}
    vector<int> actions = find_possible_actions(state, last_action);

    if(actions.size() == 0)
    {
        last_action_ = -1;
    }
    else
    {
        // Ask for I/O input
        print_possible_actions(actions);
        cout << "Player " << id_ << " : ";
        cin >> last_action_;

        // TO DO : AUTOMIZE THIS!!!!!!!!!!!!
    }
}

int Player::decrease_stack_wrt_action(int action)
{
    if(action == 1 || action == 2)
	{
        stack_--;
        return 1;
    }
    else if(action == 3)
    {
        stack_ -= 2;
        return 2;
    }
    return 0;
}


// To be used for RNN setup:
vector<vector<float>> Player::get_network_input()
{
    vector<vector<float>> info;

    // Card 1
    vector<float> temp = pocket_cards_[0].get_card_info();
    info.push_back(temp);

    // Card 2
    temp.clear();
    temp = pocket_cards_[1].get_card_info();
    info.push_back(temp);

    // Blind status
    temp.clear();
    temp.push_back(blind_type_);
    info.push_back(temp);

    return info;
}



vector<float> OneHotEncode(int index, int length)
{
    vector<float> encoded(length, 0);
    encoded[index] = 1;
    return encoded;
}
