#include "Card.h"

Card::Card()
{
	m_rank = 0; // Rank of the card
	m_suit = 0; // Suit of the card
	m_select = false; // Whether the card is selected
	m_bulk = -1; // Size of the card, A is the largest, Z is the smallest
}

Card::~Card()
{
}

char Card::GetSuit() // Get the suit of the card
{
	return m_suit;
}

void Card::SetSuit(char suit) // Set the suit of the card
{
	m_suit = suit;
}

int Card::GetRank() // Get the rank of the card
{
	return m_rank;
}

void Card::SetRank(int rank) // Set the rank of the card
{
	m_rank = rank;
}

bool Card::GetSelect() // Get the selected card
{
	return m_select;
}

void Card::SetSelect(bool select) // Set the selected card
{
	m_select = select;
}

int Card::GetBulk() // Get the size of the card
{
	return m_bulk;
}

void Card::SetBulk(int bulk) // Set the size of the card
{
	m_bulk = bulk;
}
