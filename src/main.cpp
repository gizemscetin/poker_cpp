
#include <iostream>

#include "Deck.h"
#include "Dealer.h"
#include "Game.h"

#include "PokerNetwork.h"
#include "OrganicNNet/NNET.hpp"

using namespace std;

using namespace OrganicNNet;

int main()
{
    PokerNetwork PNet;

    unsigned pop = 0;

    while (pop < 1)
    {
        pop++;
        cout << endl << "*** Population: " << pop << endl << endl;

        vector<pair<float, Organism>> population = PNet.population;

        for(int i = 0; i < population.size(); i++)
        {
            NNET *net = &(population[i].second.neural_net);
            AIPlayer pl1(net->serialize());

            for(int j = 1; j < population.size(); j++)
            {
                //cout << "\t\tAI " << i << " vs AI " << j << endl;

                NNET *net2 = &(population[j].second.neural_net);
                AIPlayer pl2(net2->serialize());

                Game G(&pl1, &pl2);
                G.start(false);

                pl1.reset_stack();
                pl1.reset_history();

                //cout << "\t***END***\tAI " << i << " vs AI " << j << "\t***END***" << endl << endl << endl;
            }
            int sum_of_earnings = pl1.compute_total_win();
            cout << "\t\tAI " << i << " stack difference : " <<  sum_of_earnings << endl;
            PNet.population[i].first = sum_of_earnings;
        }

        std::sort(
                PNet.population.begin(), PNet.population.end(),
                [](const std::pair<float, Organism> &x,
                   const std::pair<float, Organism> &y) { return x.first > y.first; });
        cout << " -Pop best: " << PNet.population[0].first << endl;

        //PNet.next_population();
    }

    NNET *net = &(PNet.population[0].second.neural_net);
    AIPlayer pl1(net->serialize());

    Player pl2;
    Game G(&pl1, &pl2);
    G.start(true);

    //Organism best(PNet.net_str);



    return 0;
}
