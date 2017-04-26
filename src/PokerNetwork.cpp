#include "PokerNetwork.h"

PokerNetwork::PokerNetwork()
{
    max_score = 0;
    population_size = 10;
    mutation_rate = 0.05;

    InputLayer *il1 = new InputLayer(4, "input_hand");
    InputLayer *il2 = new InputLayer(1, "input_blind");

    RecurrentLayer *rl1 = new RecurrentLayer(2, 10, "rec_community", ACTIVATION_TYPE::RELU, -1.0, 1.0);
    RecurrentLayer *rl2 = new RecurrentLayer(5, 8, "rec_self_act", ACTIVATION_TYPE::RELU, -1.0, 1.0);
    RecurrentLayer *rl3 = new RecurrentLayer(5, 15, "rec_opp_act", ACTIVATION_TYPE::RELU, -1.0, 1.0);
    RecurrentLayer *rl4 = new RecurrentLayer(1, 15, "rec_pot", ACTIVATION_TYPE::RELU, -1.0, 1.0);

    //cl1 = new ComputeLayer(20, "hidden1", ACTIVATION_TYPE::RELU);
    //cl2 = new ComputeLayer(10, "hidden2", ACTIVATION_TYPE::RELU);
    //cl3 = new ComputeLayer(15, "hidden1", ACTIVATION_TYPE::RELU);
    //cl4 = new ComputeLayer(20, "hidden1", ACTIVATION_TYPE::RELU);
    //cl5 = new ComputeLayer(25, "hidden1", ACTIVATION_TYPE::RELU);

    ComputeLayer *ol = new ComputeLayer(5, "output", ACTIVATION_TYPE::RELU);

    ol->link(il1, -1.0, 1.0);
    ol->link(il2, -1.0, 1.0);
    ol->link(rl1, -1.0, 1.0);
    ol->link(rl2, -1.0, 1.0);
    ol->link(rl3, -1.0, 1.0);
    ol->link(rl4, -1.0, 1.0);

    NNET net;
    net.addOutput(ol);
    net_str = net.serialize();

    for (int i = 0; i < population_size; ++i)
    {
        Organism org(net_str);
        //org.mutate(0.05, 1.0);
        pair<float, Organism> p1;
        p1.first = 0.0;
        p1.second = org;
        population.push_back(p1);
    }

    //delete il1, il2;
    //delete rl1, rl2, rl3, rl4;
    //delete ol;
}

PokerNetwork::~PokerNetwork()
{
/*
    if(il1 != nullptr) delete il1;
    if(il2 != nullptr) delete il2;
    if(rl1 != nullptr) delete rl1;
    if(rl2 != nullptr) delete rl2;
    if(rl3 != nullptr) delete rl3;
    if(rl4 != nullptr) delete rl4;
    if(cl1 != nullptr) delete cl1;
    if(cl2 != nullptr) delete cl2;
    if(cl3 != nullptr) delete cl3;
*/
}

void PokerNetwork::next_population()
{
    srand(time(NULL));

     std::sort(
                population.begin(), population.end(),
                [](const std::pair<float, Organism> &x,
                   const std::pair<float, Organism> &y) { return x.first > y.first; });

    vector<Organism> children;
    int score_sum = 0;

    for (int j = 0; j < population.size(); ++j)
    {
        score_sum += population[j].first;
    }
    //score_sum = max_score;

    children.push_back(Organism(net_str));

    double c1 = 0;
    double c2 = 0;

    for (int k = 0; k < population.size(); ++k)
    {
        c1 += ((double)population[k].first/(double)score_sum);
        if( c1 > randomFloat(0, 1))
        {
            c1 = 0;
            for (int i = 0; i < population.size(); ++i)
            {
                c2 += (double)population[i].first/(double)score_sum;
                if(c2 > randomFloat(0, 1))
                {
                    c2 = 0;
                    if(children.size()+2 < population_size)
                    {
                        auto v1 = population[k].second.reproduceWith(population[i].second);
                        children.push_back(v1[0]);
                        children.push_back(v1[1]);
                        k = 0;
                        if(children.size() > population_size*2.0/3.0)
                        {
                            k = population_size;
                        }
                    }
                    break;
                }
            }
        }
    }

    vector<pair<float, Organism>> new_population;

    for (int i = 0; i < children.size(); ++i)
    {
        children[i].mutate(mutation_rate, 100.0);
        pair<float, Organism> p1;
        p1.first = 0;
        p1.second = children[i];
        new_population.push_back(p1);
    }

    for (int i = new_population.size(); i < population_size; ++i)
    {
        Organism org(net_str);
        org.reinit(-5.0, 5.0);
        pair<float, Organism> p1;
        p1.first = 0;
        p1.second = org;
        new_population.push_back(p1);
    }

    for (int i = 0; i < population.size(); ++i)
    {
        population[i].second.neural_net.delete_all();
    }

    population = new_population;
}

