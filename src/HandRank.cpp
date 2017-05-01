#include "HandRank.h"

string HandRankIOMap[] = {"High Card", "One Pair", "Two Pairs", "Three of a Kind",
                        "Straight", "Flush", "Full House", "Four of a Kind",
                        "Straight Flush", "Royal Flush"};

vector<Card> HandRank::RoyalFlush(vector<Card> player, vector<Card> CommunityCards) {
	vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(7);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());
	sort(ordered.begin(), ordered.end());
	if (ordered[0].rank() == 1) {
		ordered.insert(ordered.end(), {14, ordered[0].suit()});
		ordered.erase(ordered.begin());
	}
	if ((StraightFlush(player, CommunityCards)[0].rank() != 0) && (ordered[2].rank() == 10) && (ordered[3].rank() == 11) && (ordered[4].rank() == 12) && (ordered[5].rank() == 13) && (ordered[6].rank() == 14)) {
		reverse(ordered.begin(), ordered.end());
		best.insert(best.begin(), ordered.begin(), ordered.begin() + 5);
		return best;
	}
	else {
		best.insert(best.begin(), 5, { 0, 0 });
		return best;
	}
}

vector<Card> HandRank::StraightFlush(vector<Card> player, vector<Card> CommunityCards) {
	vector<Card>best;
	if (((Flush(player, CommunityCards))[0].rank() != 0) && (Straight(player, CommunityCards)[0].rank() != 0) && ((Flush(player, CommunityCards))[0].rank() == (Straight(player, CommunityCards))[0].rank()) && ((Flush(player, CommunityCards))[1].rank() == (Straight(player, CommunityCards))[1].rank()) && ((Flush(player, CommunityCards))[2].rank() == (Straight(player, CommunityCards))[2].rank()) && ((Flush(player, CommunityCards))[3].rank() == (Straight(player, CommunityCards))[3].rank()) && ((Flush(player, CommunityCards))[4].rank() == (Straight(player, CommunityCards))[4].rank()) ) {
		return Straight(player, CommunityCards);
	}
	else {
		best.insert(best.begin(), 5, { 0,0 });
		return best;
	}
}

vector<Card> HandRank::FourOfAKind(vector<Card> player, vector<Card> CommunityCards)
{
    vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(7);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());

	for(int i=0; i<ordered.size(); i++)
	{
        if(ordered[i].rank() == 1)
        {
            ordered[i].set_rank(14);
        }
	}

	sort(ordered.begin(), ordered.end());
	reverse(ordered.begin(), ordered.end());

	for (int i = 0; i < 4; i++)
	{
		if (ordered[i].rank() == ordered[i + 1].rank()
            && ordered[i + 1].rank() == ordered[i + 2].rank()
            && ordered[i + 2].rank() == ordered[i + 3].rank())
		{
            best.insert(best.begin(), ordered.begin() + i, ordered.begin() + (i + 4));
            ordered.erase(ordered.begin() + i, ordered.begin() + (i + 4));
            best.push_back(ordered[0]);

            return best;
		}
	}

	best.insert(best.begin(), 5, { 0,0 });
	return best;
}

vector<Card> HandRank::FullHouse(vector<Card> player, vector<Card> CommunityCards) {
	vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(7);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());
	sort(ordered.begin(), ordered.end());
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if ((ordered[y + 2].rank() == ordered[y + 1].rank()) && (ordered[y + 1].rank() == ordered[y].rank()) && (ordered[6 - x].rank() == ordered[5 - x].rank()) && (ordered[6 - x].rank() != ordered[y + 1].rank()) && (ordered[y].rank() != ordered[5 - x].rank()) ) {
				best.insert(best.end(), 3, ordered[y]);
				best.insert(best.begin(), 2, ordered[6-x]);
				return best;
			}
			if ((ordered[x + 1].rank() == ordered[x].rank()) && (ordered[6 - y].rank() == ordered[5 - y].rank()) && (ordered[5 - y].rank() == ordered[4 - y].rank()) && (ordered[5 - y].rank() != ordered[x + 1].rank()) && (ordered[x].rank() != ordered[5 - y].rank())) {
				best.insert(best.begin(), 3, ordered[4 - y]);
				best.insert(best.end(), 2, ordered[x]);
				return best;
			}
		}
	}
	best.insert(best.begin(), 5, { 0,0 });
	return best;
}

vector<Card> HandRank::Flush(vector<Card> player, vector<Card> CommunityCards)
{
	vector<Card>best;
	best.reserve(5);
	int club = 0;
	int diamond = 0;
	int	heart = 0;
	int	spade = 0;
	vector<Card>club_cardranks;
	vector<Card>diamond_cardranks;
	vector<Card>heart_cardranks;
	vector<Card>spade_cardranks;
	vector<Card>nothing;

	for (int y = 0; y < 2; y++)
	{
		if (player[y].suit() == 1)
		{
			club++;
			club_cardranks.insert(club_cardranks.begin(), { player[y].rank(), 1 });
		}
		else if (player[y].suit() == 2)
		{
			diamond++;
			diamond_cardranks.insert(diamond_cardranks.begin(), { player[y].rank(), 2 });
		}
		else if (player[y].suit() == 3)
		{
			heart++;
			heart_cardranks.insert(heart_cardranks.begin(), { player[y].rank(), 3 });
		}
		else
		{
			spade++;
			spade_cardranks.insert(spade_cardranks.begin(), { player[y].rank(), 4 });
		}
	}
	for (int x = 0; x < 5; x++)
	{
		if (CommunityCards[x].suit() == 1)
		{
			club++;
			club_cardranks.insert(club_cardranks.begin(), { CommunityCards[x].rank() , 1 });
		}
		else if (CommunityCards[x].suit() == 2)
		{
			diamond++;
			diamond_cardranks.insert(diamond_cardranks.begin(), { CommunityCards[x].rank(), 2 });
		}
		else if (CommunityCards[x].suit() == 3)
		{
			heart++;
			heart_cardranks.insert(heart_cardranks.begin(), { CommunityCards[x].rank(), 3 });
		}
		else
		{
			spade++;
			spade_cardranks.insert(spade_cardranks.begin(), { CommunityCards[x].rank(), 4 });
		}
	}
	if (club >= 5)
	{
		sort(club_cardranks.begin(), club_cardranks.end());
		if (club_cardranks[0].rank() == 1)
			club_cardranks.push_back(Card{ 14, 1} );
		sort(club_cardranks.begin(), club_cardranks.end());
		reverse(club_cardranks.begin(), club_cardranks.end());
		best.insert(best.begin(), club_cardranks.begin(), club_cardranks.begin() + 5);
		return best;
	}
	else if (diamond >= 5) {
		sort(diamond_cardranks.begin(), diamond_cardranks.end());
		if (diamond_cardranks[0].rank() == 1)
			diamond_cardranks.push_back(Card{ 14, 2});
		sort(diamond_cardranks.begin(), diamond_cardranks.end());
		reverse(diamond_cardranks.begin(), diamond_cardranks.end());
		best.insert(best.begin(), diamond_cardranks.begin(), diamond_cardranks.begin() + 5);
		return best;
	}
	else if (heart >= 5) {
		sort(heart_cardranks.begin(), heart_cardranks.end());
		if (heart_cardranks[0].rank() == 1)
			heart_cardranks.push_back(Card{ 14, 3 });
		sort(heart_cardranks.begin(), heart_cardranks.end());
		reverse(heart_cardranks.begin(), heart_cardranks.end());
		best.insert(best.begin(), heart_cardranks.begin(), heart_cardranks.begin() + 5);
		return best;
	}
	else if (spade >= 5) {
		sort(spade_cardranks.begin(), spade_cardranks.end());
		if (spade_cardranks[0].rank() == 1)
			spade_cardranks.push_back(Card{ 14, 4 });
		sort(spade_cardranks.begin(), spade_cardranks.end());
		reverse(spade_cardranks.begin(), spade_cardranks.end());
		best.insert(best.begin(), spade_cardranks.begin(), spade_cardranks.begin() + 5);
		return best;
	}
	else {
		for (int a = 0; a < 5; a++) {
			nothing.insert(nothing.begin(), 5, { 0, 0 });
		}
		return nothing;
	}
}

vector<Card> HandRank::Straight(vector<Card> player, vector<Card> CommunityCards)
{
	vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(8);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());
	sort(ordered.begin(), ordered.end());
	if (ordered[0].rank() == 1)
		ordered.insert(ordered.end(), { 14, ordered[0].suit() });
	else
		ordered.insert(ordered.begin(), { 0, 0 });
	for (int y = 0; y < (ordered.size()-1); y++) {
		if (ordered[y].rank() == ordered[y + 1].rank())
			ordered.erase(ordered.begin()+(y+1));
	}

	for (int y = 0; y < (ordered.size()-4); y++) {
		if (((ordered[y + 1].rank() == ordered[y].rank() + 1) && (ordered[y + 2].rank() == ordered[y].rank() + 2) && (ordered[y + 3].rank() == ordered[y].rank() + 3) && (ordered[y + 4].rank() == ordered[y].rank() + 4) && (ordered[y].rank() != 0))) {
			best.push_back(ordered[y + 4]);
			best.push_back(ordered[y + 3]);
			best.push_back(ordered[y + 2]);
			best.push_back(ordered[y + 1]);
			best.push_back(ordered[y]);
			return best;
		}
	}
	for (int a = 0; a < 5; a++) {
		best.insert(best.begin(), { 0,0 });
	}
	return best;
}

vector<Card> HandRank::ThreeOfAKind(vector<Card> player, vector<Card> CommunityCards)
{
	vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(7);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());

	for(int i=0; i<ordered.size(); i++)
	{
        if(ordered[i].rank() == 1)
        {
            ordered[i].set_rank(14);
        }
	}

	sort(ordered.begin(), ordered.end());
	reverse(ordered.begin(), ordered.end());

	for (int i = 0; i < 5; i++)
	{
		if (ordered[i].rank() == ordered[i + 1].rank()
            && ordered[i + 2].rank() == ordered[i + 1].rank())
		{
            best.insert(best.begin(), ordered.begin() + i, ordered.begin() + (i + 3));
            ordered.erase(ordered.begin() + i, ordered.begin() + (i + 3));
            best.insert(best.end(), ordered.begin(), ordered.begin() + 2);

            return best;
		}
	}

	best.insert(best.begin(), 5, { 0,0 });
	return best;
}

vector<Card> HandRank::TwoPair(vector<Card> player, vector<Card> CommunityCards)
{
	vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(7);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());

	for(int i=0; i<ordered.size(); i++)
	{
        if(ordered[i].rank() == 1)
        {
            ordered[i].set_rank(14);
        }
	}

	sort(ordered.begin(), ordered.end());
	reverse(ordered.begin(), ordered.end());

	for(int i=0; i<4; i++)
	{
        if(ordered[i].rank() == ordered[i+1].rank())
        {
            for(int j=i+2; j<6; j++)
            {
                if(ordered[j].rank() == ordered[j+1].rank())
                {
                    best.insert(best.begin(), ordered.begin() + i, ordered.begin() + (i + 2));
                    best.insert(best.end(), ordered.begin() + j, ordered.begin() + (j + 2));

                    ordered.erase(ordered.begin() + i, ordered.begin() + (i + 2));
                    ordered.erase(ordered.begin() + (j - 2), ordered.begin() + j);

                    best.push_back(ordered[0]);

                    return best;
                }
            }
        }
	}

	best.insert(best.begin(), 5, { 0,0 });
	return best;
}

vector<Card> HandRank::OnePair(vector<Card> player, vector<Card> CommunityCards)
{
	vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(7);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());

	for(int i=0; i<ordered.size(); i++)
	{
        if(ordered[i].rank() == 1)
        {
            ordered[i].set_rank(14);
        }
	}

	sort(ordered.begin(), ordered.end());
	reverse(ordered.begin(), ordered.end());


	for (int i = 0; i < 6; i++)
	{
		if (ordered[i].rank() == ordered[i + 1].rank())
		{
            best.insert(best.begin(), ordered.begin() + i, ordered.begin() + (i + 2));
            ordered.erase(ordered.begin() + i, ordered.begin() + (i + 2));
            best.insert(best.end(), ordered.begin(), ordered.begin() + 3);

            return best;
		}
	}
	best.insert(best.begin(), 5, { 0,0 });
	return best;
}

vector<Card> HandRank::HighCard(vector<Card> player, vector<Card> CommunityCards)
{
	vector<Card>ordered;
	vector<Card>best;
	best.reserve(5);
	ordered.reserve(7);
	ordered.insert(ordered.end(), player.begin(), player.end());
	ordered.insert(ordered.end(), CommunityCards.begin(), CommunityCards.end());

	for(int i=0; i<ordered.size(); i++)
	{
        if(ordered[i].rank() == 1)
        {
            ordered[i].set_rank(14);
        }
	}

	sort(ordered.begin(), ordered.end());
	reverse(ordered.begin(), ordered.end());

    best.insert(best.begin(), ordered.begin(), ordered.begin() + 5);

	return best;
}
