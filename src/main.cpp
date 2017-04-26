
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
    //Game G;
    //G.start();

    PokerNetwork PNet;

    unsigned pop = 0;

    while (pop < 5)
    {
        pop++;
        cout << "Population: " << pop << endl;

        //NNET *net = &PNet.population[0].second.neural_net;
        //AIPlayer *pl1 = new AIPlayer(net->serialize());

        for (int i = 0; i < PNet.population.size(); ++i)
        {
            NNET *net = &PNet.population[i].second.neural_net;
            AIPlayer *pl1 = new AIPlayer(net->serialize());

            for(int j = 0; j < PNet.population.size(); j++)
            {
                if( i != j)
                {
                    *net = PNet.population[j].second.neural_net;
                    AIPlayer *pl2 = new AIPlayer(net->serialize());

                    Game G(pl1, pl2);
                    G.start();
                }

            }
            PNet.population[i].first = pl1->stack();
            cout << "Stack : " << pl1->stack() << endl;

        }
        //cout << " -Pop best: " << PNet.population[0].first << endl;

        //ga.next_population();
    }

    //Organism best(PNet.net_str);



    return 0;
}
