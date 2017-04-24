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
            action_set.push_back(raise);
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
            action_set.push_back(raise);
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



void Player::act(int state, int last_action)
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
