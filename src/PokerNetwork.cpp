#include "PokerNetwork.h"

PokerNetwork::PokerNetwork()
{
    max_score_ = 0;

    il1 = new InputLayer(2, "input_cards");
    il2 = new InputLayer(1, "input_blind");

    // RecurrentLayer(unsigned input_size, unsigned output_size, string name, ACTIVATION_TYPE type, float min, float max);

    rl1 = new RecurrentLayer(5, 20, "rec_community", ACTIVATION_TYPE::RELU, 0.0, 1.0);
    rl2 = new RecurrentLayer(10, 20, "rec_self_act", ACTIVATION_TYPE::RELU, 0.0, 1.0);
    rl3 = new RecurrentLayer(10, 15, "rec_opp_act", ACTIVATION_TYPE::RELU, 0.0, 1.0);

    cl1 = new ComputeLayer(20, "hidden1", ACTIVATION_TYPE::RELU);
    cl2 = new ComputeLayer(10, "hidden2", ACTIVATION_TYPE::RELU);
    cl3 = new ComputeLayer(15, "hidden1", ACTIVATION_TYPE::RELU);
    //cl4 = new ComputeLayer(20, "hidden1", ACTIVATION_TYPE::RELU);
    //cl5 = new ComputeLayer(25, "hidden1", ACTIVATION_TYPE::RELU);

    cl1->link(il1, 0.0, 1.0);
    cl1->link(il2, 0.0, 1.0);
    cl1->link(rl1, 0.0, 1.0);

    ol = new ComputeLayer(5, "output", ACTIVATION_TYPE::RELU);

    //ctor
    /*
        max_score = 0;

        InputLayer *il1 = new InputLayer(784, "input");
        ComputeLayer *cl1 = new ComputeLayer(40, "hidden1", ACTIVATION_TYPE::RELU);
        ComputeLayer *ol1 =
                new ComputeLayer(10, "output", ACTIVATION_TYPE::SIGMOID);

        ol1->link(cl1, -1.0, 1.0);
        cl1->link(il1, -1.0, 1.0);

        NNET net;
        net.addOutput(ol1);
        net_str = net.serialize();

        for (int i = 0; i < 20; ++i) {
            Organism org(net_str);
            org.mutate(0.05, 1.0);
            pair<float, Organism> p1;
            p1.first = 0.0;
            p1.second = org;
            population.push_back(p1);
        }
    */
}

PokerNetwork::~PokerNetwork()
{
    if(il1 != nullptr) delete il1;
    if(il2 != nullptr) delete il2;
    if(rl1 != nullptr) delete rl1;
    if(rl2 != nullptr) delete rl2;
    if(rl3 != nullptr) delete rl3;
    if(cl1 != nullptr) delete cl1;
    if(cl2 != nullptr) delete cl2;
    if(cl3 != nullptr) delete cl3;
}

void PokerNetwork::next_population()
{

    /*
        std::sort(
                population.begin(), population.end(),
                [](const std::pair<float, Organism> &x,
                   const std::pair<float, Organism> &y) { return x.first > y.first; });

        if (max_score < population[0].first) {
            max_score = population[0].first;
            net_str = population[0].second.neural_net.serialize();
            cout << "NEW BEST: " << max_score << endl;
            // cout << net_str << endl;
        }

        vector<Organism> children;

        auto c1 = population[0].second.reproduceWith(population[1].second);
        auto c2 = population[0].second.reproduceWith(population[2].second);
        auto c3 = population[0].second.reproduceWith(population[3].second);
        auto c4 = population[1].second.reproduceWith(population[2].second);
        auto c5 = population[1].second.reproduceWith(population[3].second);

        children.push_back(Organism(net_str));

        children.insert(children.end(), c1.begin(), c1.end());
        children.insert(children.end(), c2.begin(), c2.end());
        children.insert(children.end(), c3.begin(), c3.end());
        children.insert(children.end(), c4.begin(), c4.end());
        children.insert(children.end(), c5.begin(), c5.end());

        vector<pair<float, Organism>> new_population;

        for (int i = 0; i < children.size(); ++i) {
            children[i].mutate(0.99, 1000.0);
            pair<float, Organism> p1;
            p1.first = 0.0;
            p1.second = children[i];
            new_population.push_back(p1);
        }

        for (int i = new_population.size(); i < 20; ++i) {
            Organism org(net_str);
            org.reinit(-100.0, 100.0);
            // org.mutate(0.05, 10.0);
            pair<float, Organism> p1;
            p1.first = 0.0;
            p1.second = org;
            new_population.push_back(p1);
        }

        for (int i = 0; i < population.size(); ++i) {
            population[i].second.neural_net.delete_all();
        }

        population = new_population;
    }
    */
}

