#pragma once 

#ifndef _CARDSTACK_H_
#define _CARDSTACK_H_ 1
#include "Card.h"

enum SENDCARDSTAUS
{	
	CardErrorStaue = -1, // Passed parameter overflow is -1
	CardNotSendStaus = 0, // Not played
	CardSendStaus = 1 // Played
};

class CardStack
{
public:
	CardStack();
	virtual ~CardStack();

	inline void push(const Card card) // Push the current card
	{
		if (0 > m_count && m_count >= PLAYCARDNUM) return; // Full stack
		m_card[m_count] = card;
		m_sendCardStaus[m_count] = CardNotSendStaus;

		++m_count;
	}
	inline Card pop(void) // Pop the current card
	{
		if (0 < m_count) // If there is data in the current stack, decrement, otherwise return the last data
		     --m_count;
		m_sendCardStaus[m_count] = CardSendStaus;

		return m_card[m_count];
	}
	int GetStackSize();

	int GetSendCardStaus(int value); // Get the current card status
	int SetSendCardStaue(int value, int staus); // Set the card status
    Card GetCard(int count); // Get a card
	Card GetVauleFigure(int value); // Get the card corresponding to the passed parameter
	int GetSendCardNum(); // Get the number of played cards

private:
	int m_count; // Number of cards
	Card m_card[PLAYCARDNUM]; // Card values
	int m_sendCardStaus[PLAYCARDNUM]; // Whether the card is played, FALSE: the card is in the stack and not played, TRUE: the card is in the stack but already played
	int m_sendCardNum; // Number of played cards
};

#endif
