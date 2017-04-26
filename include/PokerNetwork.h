#ifndef POKERNETWORK_H
#define POKERNETWORK_H

#include <iostream>
#include "OrganicNNet/Organism.hpp"
#include "OrganicNNet/RandomFloat.hpp"

using namespace std;
using namespace OrganicNNet;

class PokerNetwork
{
    public:
        PokerNetwork();
        ~PokerNetwork();

        void next_population();

        float max_score;
        string net_str;
        float mutation_rate;
        unsigned population_size;

        vector<pair<float, Organism>> population;
};

#endif // POKERNETWORK_H
