#ifndef PLAYER_H
#define PLAYER_H

#include "Deck.h"
#include "PokerNetwork.h"

using namespace OrganicNNet;

typedef enum Action
{
    check = 0,
    call = 1,
    bet = 2,
    Raise = 3,
    fold = 4
}Action;

vector<float> OneHotEncode(int index, int length);

class Player
{
    public:
        Player() {};
        Player(int id) : id_(id) {};

        void set_id(int id) { id_ = id; };
        void set_last_action(int action) { last_action_ = action; };
        void set_cards(vector<Card> cards) { pocket_cards_ = cards; };
        void add_card(Card card) { pocket_cards_.push_back(card); };
        void clear_cards() { pocket_cards_.clear(); };
        void show_pockets();
        void set_blind_type(int blind) { blind_type_ = blind; decrease_stack(blind+1);};
        void increase_stack(int cnt) { stack_ += cnt; };
        void decrease_stack(int cnt) { stack_ -= cnt; };
        int decrease_stack_wrt_action(int action);

        bool check_id(int id) { return (id == id_); };
        bool lost() { return (stack_ <= 0); };


        int last_action() const { return last_action_; };
        void print_possible_actions(vector<int> actions);
        void print_last_action();
        vector<int> find_possible_actions(int state, int last_action);
        //virtual void act(int state, int last_action);

        void add_action(int action) { action_history_.push_back(action); };

        int id() const { return id_; };
        int stack() const { return stack_; };
        int blind_type() const { return blind_type_; };
        vector<Card> pocket_cards() const { return pocket_cards_; };
        vector<int> action_history() const { return action_history_; };

        friend ostream& operator<<(ostream& os, const Player& player)
        {
            os << "[Player " << player.id() << " : " << player.stack() << "]";
            return os;
        };


        // To be used for RNN setup:
        vector<vector<float>> get_network_input();
        virtual void act(vector<Card> community_cards,
                       vector<int> opponent_history, int current_pot, int current_state);

    protected:
    private:

        int id_; // 1 or 2
        vector<Card> pocket_cards_;
		int stack_ = 1000;
		vector<int> action_history_;
		int last_action_ = -1;
		int blind_type_; // 0 for small, 1 for big
		int raise_ctr_;
};





#endif // PLAYER_H
