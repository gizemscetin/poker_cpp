#ifndef GAME_H
#define GAME_H

#include "Dealer.h"
#include "Player.h"

class Game
{
    public:
        Game(int player_count = 2);
        virtual ~Game();

        int pot() const { return pot_; };

        void show_status();
        void show_player_pockets();

        void start();
        void play_one_round();
        bool play_one_state(int state);

        void update_winner_stack();

        friend ostream& operator<<(ostream& os, const Game& game)
        {
            os << "\n\t[Pot: " << game.pot() << "]";
            return os;
        };
    protected:
    private:
        vector<Player> players_;
        int pot_ = 0;
        int winner_id_;
};

#endif // GAME_H