#include "AIPlayer.h"


AIPlayer::AIPlayer(string net) : Player()
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

void AIPlayer::act(vector<Card> community_cards, vector<int> opponent_history,
                   int last_action, int current_pot, int state)
{
    // find possible moves
    auto action_set = find_possible_actions(state, last_action);
    //cout << "#Possible actions : " << action_set.size() << endl;

    try
    {

        if(action_set.size() != 0)
        {
            //print_possible_actions(action_set);
            // Player's Inputs
            vector<float> input_hand ;//= pocket_cards()[0].get_card_info();
            input_hand.push_back(pocket_cards()[0].rank());
            input_hand.push_back(pocket_cards()[0].suit());
            input_hand.push_back(pocket_cards()[1].rank());
            input_hand.push_back(pocket_cards()[1].suit());

            int err = network->all_layers["input_hand"]->setActivations(input_hand);
            if(err != OrganicNNet::GeneralError::None){
                cout << OrganicNNet::GeneralError::what(err) << endl;
                exit(12);
            }

            vector<float> input_blind;
            input_blind.push_back(blind_type());
            err = network->all_layers["input_blind"]->setActivations(input_blind);
            if(err != OrganicNNet::GeneralError::None){
                cout << OrganicNNet::GeneralError::what(err) << endl;
                exit(13);
            }


            // rl4
            vector<vector<float>> rec_pot(1);
            rec_pot[0].push_back(current_pot);
            err = ((RecurrentLayer*)network->all_layers["rec_pot"])->setInputs(rec_pot);
            if(err != OrganicNNet::GeneralError::None){
                cout << OrganicNNet::GeneralError::what(err) << endl;
                exit(14);
            }

            // rl1
            vector<vector<float>> rec_community;
            for(int i=0; i<community_cards.size(); i++)
            {
                rec_community.push_back(community_cards[i].get_card_info());
            }
            err = ((RecurrentLayer*)network->all_layers["rec_community"])->setInputs(rec_community);
            if(err != OrganicNNet::GeneralError::None){
                cout << OrganicNNet::GeneralError::what(err) << endl;
                exit(15);
            }

            // rl2
            vector<vector<float>> rec_self_act;
            for(int i=0; i<action_history().size(); i++)
            {
                rec_self_act.push_back(OneHotEncode(action_history()[i], 5));
            }
            err = ((RecurrentLayer*)network->all_layers["rec_self_act"])->setInputs(rec_self_act);
            if(err != OrganicNNet::GeneralError::None){
                cout << OrganicNNet::GeneralError::what(err) << endl;
                exit(16);
            }

            // rl3
            vector<vector<float>> rec_opp_act;
            for(int i=0; i<opponent_history.size(); i++)
            {
                rec_opp_act.push_back(OneHotEncode(opponent_history[i], 5));
            }
            err = ((RecurrentLayer*)network->all_layers["rec_opp_act"])->setInputs(rec_opp_act);
            if(err != OrganicNNet::GeneralError::None){
                cout << OrganicNNet::GeneralError::what(err) << endl;
                exit(17);
            }

            // OUTPUT
            //cout << 1 << endl;
            network->all_layers["output"]->fire();
            //cout << 2 << endl;
            auto v = network->all_layers["output"]->getActivations();
            //cout << 3 << endl;
            network->all_layers["output"]->reset();
            //cout << 4 << endl;

            //cout << "Activation size : " << v.size() << endl;


            int maxx = 0;
            int action;

            action = action_set[0];
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
        else
            set_last_action(-1);
    }
    catch(exception &e)
    {
        cout << "Act problem." << endl;
    }
}
