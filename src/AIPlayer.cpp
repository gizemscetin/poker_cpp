#include "AIPlayer.h"


AIPlayer::AIPlayer(string net)
{
    //ctor
    network = new NNET();
    network->read(net);

}

AIPlayer::~AIPlayer()
{
    //dtor
    network->delete_all();
}

void AIPlayer::act(vector<Card> community_cards,
                vector<int> opponent_history, int current_pot, int state)
{
    // find possible moves
    auto action_set = find_possible_actions(state, *opponent_history.end());
    if(action_set.size() != 0)
    {
        // Player's Inputs
        vector<float> input_hand = pocket_cards()[0].get_card_info();
        input_hand.insert(input_hand.end(), pocket_cards()[1].get_card_info().begin(),
                          pocket_cards()[1].get_card_info().end());
        network->all_layers["input_hand"]->setActivations(input_hand);

        vector<float> input_blind;
        input_blind.push_back(blind_type());
        network->all_layers["input_blind"]->setActivations(input_blind);

        // Game related stuff

        // rl4
        vector<vector<float>> rec_pot(1);
        rec_pot[0].push_back(current_pot);
        ((RecurrentLayer*)network->all_layers["rec_pot"])->setInputs(rec_pot);

        // rl1
        vector<vector<float>> rec_community;
        for(int i=0; i<community_cards.size(); i++)
        {
            rec_community.push_back(community_cards[i].get_card_info());
        }
        ((RecurrentLayer*)network->all_layers["rec_community"])->setInputs(rec_community);

        // rl2
        vector<vector<float>> rec_self_act;
        for(int i=0; i<action_history().size(); i++)
        {
            rec_self_act.push_back(OneHotEncode(action_history()[i], 5));
        }
        ((RecurrentLayer*)network->all_layers["rec_self_act"])->setInputs(rec_self_act);

        // rl3
        vector<vector<float>> rec_opp_act;
        for(int i=0; i<opponent_history.size(); i++)
        {
            rec_opp_act.push_back(OneHotEncode(opponent_history[i], 5));
        }
        ((RecurrentLayer*)network->all_layers["rec_opp_act"])->setInputs(rec_opp_act);


        // OUTPUT
        network->all_layers["output"]->fire();
        auto v = network->all_layers["output"]->getActivations();
        network->all_layers["output"]->reset();

        int maxx = 0;
        int action;
        for(int i=0; i<action_set.size(); i++)
        {
            int index = action_set[i];
            if(v[index] > maxx)
            {
                maxx = v[index];
                action = index;
            }
        }
        set_last_action(action);
    }
}
