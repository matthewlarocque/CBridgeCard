#include "CardStack.h"
#include <Windows.h>

CardStack::CardStack()
{
	m_count = 0;
	m_sendCardNum = 0;
	memset(m_card, 0, sizeof(Card) * PLAYCARDNUM);
	memset(m_sendCardStaus, 0, sizeof(int) * PLAYCARDNUM);
}

CardStack::~CardStack()
{
}

int CardStack::GetStackSize()
{
	return m_count;
}

Card CardStack::GetCard(int count)
{
	return m_card[count];
}

int CardStack::GetSendCardStaus(int value) // Get the current card status
{
	if (0 < value && value <= m_count) // Not overflow
		return m_sendCardStaus[value - 1];
	else
		return CardErrorStaue;
}

int CardStack::SetSendCardStaue(int value, int staus) // Set the card status
{
	if ((0 < value && value <= m_count) && (CardErrorStaue < staus && staus <= CardSendStaus)) // Not overflow
	{
		if (CardSendStaus == staus && CardSendStaus != GetSendCardStaus(value))
			m_sendCardNum++;
		m_sendCardStaus[value - 1] = staus;
		return staus;
	}
	else
		return CardErrorStaue;
}

Card CardStack::GetVauleFigure(int value) // Get the card corresponding to the passed parameter value, if the parameter overflows, return 0
{
	if (m_count < value && value <= 0) // Passed variable overflow
	{
		Card m_nullCard;
		m_nullCard.SetSuit(0);
		m_nullCard.SetRank(0);
		m_nullCard.SetSelect(0);
		m_nullCard.SetBulk(-1);

		return m_nullCard;
	}
	else
		return m_card[value - 1];
}

int CardStack::GetSendCardNum()
{
	return m_sendCardNum;
}
