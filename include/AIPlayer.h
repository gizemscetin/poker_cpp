#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"
#include "OrganicNNet/NNET.hpp"

using namespace OrganicNNet;

class AIPlayer : public Player
{
    public:
        AIPlayer(string net);
        virtual ~AIPlayer();

        virtual void act(vector<Card> community_cards,
                       vector<int> opponent_history, int current_pot, int current_state);

    protected:

    private:
        NNET *network;
};

#endif // AIPLAYER_H
