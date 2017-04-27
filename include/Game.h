#ifndef GAME_H
#define GAME_H

#include "Dealer.h"
#include "HandRank.h"

#include "PokerNetwork.h"

using namespace OrganicNNet;

class Game
{
    public:
        //Game(int player_count = 2);
        Game(Player *pl1, Player *pl2);

        vector<Player*> players() const { return players_; };

        int pot() const { return pot_; };

        void show_status();
        void show_player_pockets();

        void start(bool io_on);
        void play_one_round(bool io_on);
        bool play_one_state(vector<Card> community_cards, int state, bool io_on);

        int find_winner(vector<Card> P1, vector<Card> P2, vector<Card> CommunityCards, bool io_on);
        void update_winner_stack();

        friend ostream& operator<<(ostream& os, const Game& game)
        {
            os << "\n\t[Pot: " << game.pot() << "]";
            return os;
        };


        // NETWORK
        //void player_act(NNET *network, int state);

    protected:
    private:
        vector<Player*> players_;
        int pot_ = 0;
        int winner_id_;
        PokerNetwork network_;
};

#endif // GAME_H
