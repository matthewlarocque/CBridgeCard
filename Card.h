#pragma once
#ifndef _CARD_H_
#define _CARD_H_ 1

#define CARDNUM 52          // Number of cards
#define PLAYNUM 4           // Number of players
#define PLAYCARDNUM CARDNUM / PLAYNUM // Number of cards in each player's hand
#define SUITNUM 4           // Number of suits

class Card
{
public:
	Card();
	virtual ~Card();

	char GetSuit(void);
	void SetSuit(char suit);

	int GetRank(void);
	void SetRank(int rank);

	bool GetSelect(void);
	void SetSelect(bool select);

	int GetBulk(void);
	void SetBulk(int bulk);
	
private:
	char m_suit;      // Suit of the card
	int m_rank;       // Rank of the card
	bool m_select;    // Whether the card is selected
	int m_bulk;       // Size of the card, A is the largest, Z is the smallest
};

#endif
