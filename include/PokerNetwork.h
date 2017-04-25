#ifndef POKERNETWORK_H
#define POKERNETWORK_H

#include <iostream>
#include "OrganicNNet/Organism.hpp"

using namespace std;
using namespace OrganicNNet;

class PokerNetwork
{
    public:
        PokerNetwork();
        ~PokerNetwork();

        void next_population();

    protected:
    private:

        float max_score_;
        string net_str_;

        vector<pair<float, Organism>> population_;

        InputLayer *il1 = nullptr, *il2 = nullptr;
        RecurrentLayer *rl1 = nullptr, *rl2 = nullptr, *rl3 = nullptr;
        ComputeLayer *cl1 = nullptr, *cl2 = nullptr, *cl3 = nullptr, *cl4 = nullptr, *cl5 = nullptr;
        ComputeLayer *ol = nullptr;

};

#endif // POKERNETWORK_H
