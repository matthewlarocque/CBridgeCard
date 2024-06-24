#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "Play.h"

Play::Play()
{
	srand((unsigned int)time(NULL)); // Set random variable seed
	memset(m_suitAndRankCall, 0, sizeof(SUITANDRANK) * PLAYNUM * MAXCALLTONNUM);
	memset(m_previousCardCall, 0, sizeof(SUITANDRANK) * PLAYNUM);
	memset(m_suitAndRankCallPosition, 0, sizeof(int) * PLAYNUM);
	memset(m_userOutCard, 0, sizeof(Card) * PLAYNUM);
}

Play::~Play()
{
}

void Play::SendCard()
{
	bool m_vauleCards[53] = {FALSE}; // A total of 52 cards, 0-52, excluding jokers, have been generated as TRUE
	int m_randNumber = 0;			 // Among 52 cards, the randomly generated card number

	m_vauleCards[52] = TRUE; // The extra last card is default generated

	for (int j = 0; j < PLAYCARDNUM; j++) // Each player gets 13 cards
		for (int i = 0; i < PLAYNUM; i++) // 4 players
		{
			Card &m_card = m_playCard[i][j]; // Generated card

			m_randNumber = 52;
			while (m_vauleCards[m_randNumber]) // Randomly generate 52 numbers
				//{
				m_randNumber = rand() % 52;

			m_vauleCards[m_randNumber] = TRUE;			 // Current number has been generated
			m_card.SetSelect(FALSE);					 // Not selected, selected as TRUE
			m_card.SetRank(m_randNumber % 13 + 1);		 // Card face value
			m_card.SetSuit(char(m_randNumber / 13 + 3)); // Card suit

			if (1 == m_card.GetRank()) // 1 means the face value A is the highest
			{
				m_card.SetBulk(m_card.GetRank() + 11); // Indicates the size of the card is 12
			}
			else
				m_card.SetBulk(m_card.GetRank() - 2); // Others minus 2, indicating normal size
													  //}
		}
}

void Play::SortCard(Card card[]) // Sort the cards
{
	Card m_swapData;
	for (int i = 0; i < PLAYCARDNUM; i++) // Cards are sorted by Hearts, Diamonds, Clubs, Spades, and respective big cards are in front
		for (int j = i + 1; j < PLAYCARDNUM; j++)
			if (card[i].GetSuit() < card[j].GetSuit() || (card[i].GetSuit() == card[j].GetSuit() && card[j].GetBulk() > card[i].GetBulk()))
			{
				m_swapData = card[i];
				card[i] = card[j];
				card[j] = m_swapData;
			}
}

int Play::GetPlayerPointsAndShape(Card card[], int player) // Calculate the points of the player's big cards (A, K, Q, J)
{
	int m_pointsAndShape = 0;												 // Record points
	int m_suitSpade = 0, m_suitHeart = 0, m_suitClub = 0, m_suitDiamond = 0; // Record the number of Spades, Hearts, Clubs, Diamonds
	for (int i = 0; i < PLAYCARDNUM; i++)
	{
		switch (card[i].GetRank()) // A adds 4 points, K adds 3 points, Q adds 2 points, J adds 1 point
		{						   // A adds 4 points, K adds 3 points, Q adds 2 points, J adds 1 point
		case 1:
			m_pointsAndShape += 4;
			break;
		case 13:
			m_pointsAndShape += 3;
			break;
		case 12:
			m_pointsAndShape += 2;
			break;
		case 11:
			m_pointsAndShape += 1;
			break;
		default:
			break;
		}
		/// Count the number of each suit, if a suit has 0 or 1, add 3 or 2 points respectively
		switch (card[i].GetSuit())
		{
		case char(6):
			m_cardStack[player - 1][0].push(card[i]); // Spade into the stack
			m_suitSpade++;
			break;
		case char(3):
			m_cardStack[player - 1][1].push(card[i]); // Heart into the stack
			m_suitHeart++;
			break;
		case char(4):
			m_cardStack[player - 1][2].push(card[i]); // Diamond into the stack
			m_suitDiamond++;
			break;
		case char(5):
			m_cardStack[player - 1][3].push(card[i]); // Club into the stack
			m_suitClub++;
			break;

		default:
			break;
		}
	}
	// If a suit has 0 or 1, add 3 or 2 points respectively, use this as a reference to make bids
	if (0 == m_suitSpade || 0 == m_suitHeart || 0 == m_suitClub || 0 == m_suitDiamond)
		m_pointsAndShape += 3;
	if (1 == m_suitSpade || 1 == m_suitHeart || 1 == m_suitClub || 1 == m_suitDiamond)
		m_pointsAndShape += 2;

	// Record the current player's card and attributes
	m_suitStatistic[player - 1].SetPlayer(player);
	m_suitStatistic[player - 1].SetPointsAndShapeFigure(m_pointsAndShape);
	m_suitStatistic[player - 1].SetSuitSpadeNum(m_suitSpade);
	m_suitStatistic[player - 1].SetSuitHeartNum(m_suitHeart);
	m_suitStatistic[player - 1].SetSuitClubNum(m_suitClub);
	m_suitStatistic[player - 1].SetSuitDiamondNum(m_suitDiamond);

	return m_pointsAndShape;
}

void Play::SortAndGetPlayerPAS() // Sort the cards and get the corresponding points
{
	for (int i = 0; i < PLAYNUM; i++)
	{
		SortCard(m_playCard[i]);
		GetPlayerPointsAndShape(m_playCard[i], i + 1);
	}
}

CardStack *Play::GetCardStack(int player)
{

	if (0 < player && player <= PLAYNUM) // Within bounds, return the corresponding player's stack of cards
		return m_cardStack[player - 1];
	else
		return m_cardStack[0]; // Out of bounds, return the first player's cards
}

int Play::SortCardSuitNum(SuitStatistic &suitStatistic, int order) // Arrange the number of 4 suits from most to least
{
	SUITANDSIZE m_suitSize[SUITNUM];
	SUITANDSIZE m_suitSwap;

	memset(m_suitSize, 0, SUITNUM * sizeof(SUITANDSIZE));
	memset(&m_suitSwap, 0, sizeof(SUITANDSIZE));

	m_suitSize[Spade - 1].size = suitStatistic.GetSuitSpadeNum(); // Get the number of each suit
	m_suitSize[Spade - 1].suit = char(6);

	m_suitSize[Heart - 1].size = suitStatistic.GetSuitHeartNum();
	m_suitSize[Heart - 1].suit = char(3);

	m_suitSize[Diamond - 1].size = suitStatistic.GetSuitDiamondNum();
	m_suitSize[Diamond - 1].suit = char(4);

	m_suitSize[Club - 1].size = suitStatistic.GetSuitClubNum();
	m_suitSize[Club - 1].suit = char(5);

	for (int i = 0; i < SUITNUM; i++) // Sort the number of suits
	{
		for (int j = i; j < SUITNUM; j++)
		{
			if (m_suitSize[j].size > m_suitSize[i].size)
			{
				m_suitSwap.size = m_suitSize[j].size; // Swap data, the suit with the most cards is in front
				m_suitSwap.suit = m_suitSize[j].suit;

				m_suitSize[j].size = m_suitSize[i].size;
				m_suitSize[j].suit = m_suitSize[i].suit;

				m_suitSize[i].size = m_suitSwap.size;
				m_suitSize[i].suit = m_suitSwap.suit;
			}
		}
	}

	switch (m_suitSize[order].suit) // Return the number of corresponding suits, determined by order, 0 is the most suits
	{
	case char(6):
		return Spade;
	case char(3):
		return Heart;
	case char(4):
		return Diamond;
	case char(5):
		return Club;
	default:
		return SuitError;
	}

	return SuitError;
	// return ;
}

void Play::UserSuitCallCard(int play, int rankTon, int suitAmount) // Four suits call card record, pass in RANK, SUIT
{
	switch (suitAmount)
	{
	case Spade:

		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = FALSE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = char(6);
		m_suitAndRankCallPosition[play - 1]++;
		break;
	case Heart:
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = FALSE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = char(3);
		m_suitAndRankCallPosition[play - 1]++;
		break;
	case Diamond:
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = FALSE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = char(4);
		m_suitAndRankCallPosition[play - 1]++;

		break;
	case Club:
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = FALSE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = char(5);
		m_suitAndRankCallPosition[play - 1]++;

		break;

	case SuitNT:																	   // NT
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = FALSE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'N';
		m_suitAndRankCallPosition[play - 1]++;
		break;
	case Double:																	   // Double 0D
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = FALSE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'D';
		m_suitAndRankCallPosition[play - 1]++;
		break;
	case DDouble:																	   // Redouble  1D
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = FALSE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'B';
		m_suitAndRankCallPosition[play - 1]++;
		break;
	case Pass:																		  //
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = TRUE; // Record the current user's bid
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = rankTon;
		m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'P';
		m_suitAndRankCallPosition[play - 1]++;
		break;

	default:
		break;
	}
	m_previousCardCall[play - 1].pass = m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1] - 1].pass;
	m_previousCardCall[play - 1].rank = m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1] - 1].rank;
	m_previousCardCall[play - 1].suit = m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1] - 1].suit;
}

int Play::CardCall(SuitStatistic &suitStatistic, int play /*,SUITANDRANK *previousCardCall*/)
{
	// SUITANDRANK m_localSuitAndRank; //Mark whether the data is normal
	int m_suitAmount = 0;			  // Record the most numerous suit
	int m_preCallCardPlayer = 0;	  // Previous bidding player
	int m_passCount = 0;			  // Number of pass
	static int m_firstCallPlayer = 0; // The first player to search for PASS

	if (play != suitStatistic.GetPlayer()) // Bidding error
		return false;

	// The current player is PLAY - 1, the previous player is PLAY - 1 -1. When PLAY - 1 -1< 0, the previous player is player 4
	m_preCallCardPlayer = play - 1 - 1;
	if (m_preCallCardPlayer < 0)
	{
		m_preCallCardPlayer = 4 + m_preCallCardPlayer;
	}

	if ((0 == m_previousCardCall[m_preCallCardPlayer].rank && 0 == m_previousCardCall[m_preCallCardPlayer].suit) && (false == m_previousCardCall[m_preCallCardPlayer].pass)) // First bid
	{
		if (13 <= suitStatistic.GetPointsAndShapeFigure() && 22 >= suitStatistic.GetPointsAndShapeFigure() && TRUE != m_previousCardCall[m_preCallCardPlayer].pass)
		{													  // Card points between 13 and 22, first bid, bid the most numerous suit
			m_suitAmount = SortCardSuitNum(suitStatistic, 0); // Get the most numerous suit

			if (SuitError == m_suitAmount)
				return CallError;

			if (m_previousCardCall[m_preCallCardPlayer].rank + 1 > 7)
			{
				m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = TRUE; // Record the current user's bid as PASS
				m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = 8;
				m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'P';
				m_suitAndRankCallPosition[play - 1]++; // Current user's bid count increases by 1
				m_passCount++;
			}
			else
			{
				m_passCount = 0; // When a bid is encountered, the corresponding PASS count is recalculated

				UserSuitCallCard(play, m_previousCardCall[m_preCallCardPlayer].rank + 1, m_suitAmount); // Bid the most numerous suit and record it
			}
		}
		else if (13 > suitStatistic.GetPointsAndShapeFigure())
		{ // Points less than 13, PASS

			m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = TRUE; // Record the current user's bid as PASS
			m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = 8;
			m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'P';
			m_suitAndRankCallPosition[play - 1]++; // Current user's bid count increases by 1
			m_passCount++;
		}
	}
	else // Not the first bid
	{
		m_suitAmount = SortCardSuitNum(suitStatistic, 0); // Get the most numerous suit

		// if (play < 2 && play > 4)
		//	return;
		if (SuitError == m_suitAmount)
			return CallError;

		for (int i = 0; i < 3; i++)
		{
			if ((8 == m_previousCardCall[m_preCallCardPlayer].rank && 'P' == m_previousCardCall[m_preCallCardPlayer].suit) && true == m_previousCardCall[m_preCallCardPlayer].pass) // The previous user PASS, continue to find the previous user until the previous user is itself
			{

				m_preCallCardPlayer -= 1;
				if (m_preCallCardPlayer < 0)
				{
					m_preCallCardPlayer = 4 + m_preCallCardPlayer;
				}
				m_passCount++;
			}
			if ((0 < m_previousCardCall[m_preCallCardPlayer].rank && 0 < m_previousCardCall[m_preCallCardPlayer].suit) && false == m_previousCardCall[m_preCallCardPlayer].pass && m_firstCallPlayer != m_preCallCardPlayer)
			{
				m_passCount = 0;

				m_firstCallPlayer = m_preCallCardPlayer;

				break; // Find the previous non-PASS, exit the loop
			}
		}

		if (13 > suitStatistic.GetPointsAndShapeFigure())
		{ // Points less than 13, PASS

			m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = TRUE; // Record the current user's bid as PASS
			m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = 8;
			m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'P';
			m_suitAndRankCallPosition[play - 1]++; // Current user's bid count increases by 1
			m_passCount++;
		}

		if (3 == m_passCount) // 3 players PASS
		{
			return CallOver;
		}
		else if (4 == m_passCount)
		{
			return CallAllPass;
		}
		else
			m_passCount = 0;

		if (13 <= suitStatistic.GetPointsAndShapeFigure() && 22 >= suitStatistic.GetPointsAndShapeFigure())
		{													  // Card points between 13 and 22, bid the most numerous suit
			m_suitAmount = SortCardSuitNum(suitStatistic, 0); // Get the most numerous suit

			if (SuitError == m_suitAmount)
				return CallError;
			if (m_previousCardCall[m_preCallCardPlayer].rank + 1 > 7)
			{
				m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].pass = TRUE; // Record the current user's bid as PASS
				m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].rank = 8;
				m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1]].suit = 'P';
				m_suitAndRankCallPosition[play - 1]++; // Current user's bid count increases by 1
				m_passCount++;
			}
			else
			{
				m_passCount = 0; // When a bid is encountered, the corresponding PASS count is recalculated

				UserSuitCallCard(play, m_previousCardCall[m_preCallCardPlayer].rank + 1, m_suitAmount); // Bid the most numerous suit and record it
			}
		}
	}

	// Record the current player's card to the m_previousCardCall array, bidding completed, becomes the previous player
	m_previousCardCall[play - 1].suit = m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1] - 1].suit;
	m_previousCardCall[play - 1].rank = m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1] - 1].rank;
	m_previousCardCall[play - 1].pass = m_suitAndRankCall[play - 1][m_suitAndRankCallPosition[play - 1] - 1].pass;

	return CallRunning;
}

int Play::CallCardBegin(int play)
{
	return CardCall(m_suitStatistic[play], play + 1);
}

SUITANDRANK Play::GetSuitAndRankCall(int play, int cardNumber) // Get the current player, corresponding bid number
{
	return m_suitAndRankCall[play][cardNumber];
}
int Play::GetSuitAndRankCallPosition(int play) // Get the number of bids for the current player
{
	return m_suitAndRankCallPosition[play];
}

Card Play::GetUserOutCard(int player) // Get the card played by the corresponding player
{
	return m_userOutCard[player - 1];
}

void Play::SetUserOutCard(int player, Card card) // Record the card played by the player
{
	memcpy(&m_userOutCard[player - 1], &card, sizeof(Card));
}
int Play::CompareSizeOfOutCard() // The largest of the 4 cards
{
	Card m_zeroCard;
	int m_maxCard = 0;
	for (int i = 0; i < PLAYNUM - 1; i++)
	{
		if (m_userOutCard[m_maxCard].GetSuit() == m_userOutCard[i + 1].GetSuit())
		{
			if (m_userOutCard[m_maxCard].GetBulk() < m_userOutCard[i + 1].GetBulk())
			{
				m_maxCard = i + 1;
			}
		}
	}

	{
		memset(&m_zeroCard, 0, sizeof(Card));
		m_zeroCard.SetSelect(true); // Mark that it is not the first card of the entire game
		SetUserOutCard(1, m_zeroCard);
		SetUserOutCard(2, m_zeroCard);
		SetUserOutCard(3, m_zeroCard);
		SetUserOutCard(4, m_zeroCard);
	}
	return m_maxCard;
}

void Play::ShowCard()
{
	int m_getPoint[4] = {0};
	using namespace std;
	for (int i = 0; i < PLAYNUM; i++) // Display the card data before sorting
	{
		cout << "Play" << i + 1 << " ";
		for (int j = 0; j < PLAYCARDNUM; j++)
		{
			cout << m_playCard[i][j].GetSuit() << m_playCard[i][j].GetRank() << "  ";
		}
		cout << endl;
	}
	for (int i = 0; i < 4; i++)
	{
		SortCard(m_playCard[i]); // Sort the data
		m_getPoint[i] = GetPlayerPointsAndShape(m_playCard[i], i + 1);
	}

	cout << endl;
	for (int i = 0; i < PLAYNUM; i++) // Output the sorted data
	{
		cout << "Play" << i + 1 << " " << "Point: " << m_getPoint[i] << "  ";
		for (int j = 0; j < PLAYCARDNUM; j++)
		{

			cout << m_playCard[i][j].GetSuit() << m_playCard[i][j].GetRank() << "  ";
		}
		cout << endl;
	}
	cout << endl;

	for (int i = 0; i < PLAYNUM; i++) // Output the sorted data
	{
		cout << "Play" << i + 1 << " " << "Point: " << m_suitStatistic[i].GetPointsAndShapeFigure() << "  ";

		for (int j = 0; j < 4; j++)
		{
			int m_cardLen = m_cardStack[i][j].GetStackSize();
			for (int s = 0; s < m_cardLen; s++)
			{
				Card m_getCard = m_cardStack[i][j].pop();
				cout << m_getCard.GetSuit() << m_getCard.GetRank() << "  ";
			}
		}
		cout << endl;
	}

	for (int i = 0; i < PLAYNUM; i++)
	{
		CardCall(m_suitStatistic[i], i + 1);
	}
}
