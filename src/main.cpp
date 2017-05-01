
#include <iostream>
#include <fstream>

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
    unsigned int max_pop = 1000;

    while (pop < max_pop)
    {
        pop++;
        cout << endl << "*** Population: " << pop << endl << endl;

        vector<pair<float, Organism>> population = PNet.population;

        for(int i = 0; i < population.size(); i++)
        {
            NNET *net = &(population[i].second.neural_net);
            AIPlayer pl1(net->serialize());

            for(int j = 0; j < population.size(); j++)
            {
                //cout << "\t\tAI " << i << " vs AI " << j << endl;

                if( i != j)
                {
                    NNET *net2 = &(population[j].second.neural_net);
                    AIPlayer pl2(net2->serialize());

                    Game G(&pl1, &pl2);
                    G.start(false, 10);

                    //pl1.print_action_history();
                    pl1.reset_stack();
                    pl1.reset_history();
                }

                //cout << "\t***END***\tAI " << i << " vs AI " << j << "\t***END***" << endl << endl << endl;
            }
            int sum_of_earnings = pl1.compute_total_win();
            cout << "\t\tAI " << i << " stack difference : " <<  sum_of_earnings << endl;
            PNet.population[i].first = sum_of_earnings;
        }

        if(pop < max_pop)
            PNet.next_population();
    }

    //Organism best(PNet.net_str);

    std::sort(
                PNet.population.begin(), PNet.population.end(),
                [](const std::pair<float, Organism> &x,
                   const std::pair<float, Organism> &y) { return x.first > y.first; });

   NNET *net = &(PNet.population[0].second.neural_net);


    ofstream myfile;
    myfile.open ("net_" + std::to_string(max_pop) +".txt");
    myfile << net->serialize();
    myfile.close();

    //NNET *net = &best.neural_net;

    vector<int> results;

    for(int i=0; i<10; i++)
    {
        AIPlayer pl1(net->serialize());

        Player pl2;
        Game G(&pl1, &pl2);
        G.start(true, 10);

        int sum_of_earnings = pl1.compute_total_win();
        cout << "AI stack difference : " <<  sum_of_earnings << endl;
        results.push_back(sum_of_earnings);
    }

    myfile.open("out_" + std::to_string(max_pop) +".txt");
    for(int i=0; i<10; i++)
        myfile <<results[i] << endl;
    myfile.close();

/*
    Player pl1, pl2;
    Game G(&pl1, &pl2);
    G.start(true);
*/
    return 0;
}
