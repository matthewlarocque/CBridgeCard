#pragma once

#ifndef _PLAY_H_
#define _PLAY_H_ 1
#include "Card.h"
#include "Console.h"
#include "SuitStatistic.h"
#include "CardStack.h"

#define MAXCALLTONNUM 100

enum SuitSize
{
	SuitError = 0,
	Spade = 1,
	Heart = 2,
	Diamond = 3,
	Club = 4,
	SuitNT,
	Double,
	DDouble,
	Pass
};

enum CardStatus
{
	CallRunning = 0,
	CallOver = 1,
	CallAllPass = 2,
	CallError = 7
};

typedef struct _SUITANDRANK
{
	char suit;
	int rank;
	bool pass;
} SUITANDRANK, *PSUITANDRANK;

typedef struct _SUITANDSIZE
{
	char suit; // Suit
	int size;  // Amount
} SUITANDSIZE, *PSUITANDSIZE;

class Play
{
public:
	Play();
	// Play(ConsoleLib * console);
	virtual ~Play();
	void GameStart(); // Game start

	void SendCard();									  // Deal cards
	void SortCard(Card card[]);							  // Sort cards, same suit together and higher cards first
	int GetPlayerPointsAndShape(Card card[], int player); // Calculate player points, summing cards above 10
	void SortAndGetPlayerPAS();							  // Call SortCard and GetPlayerPointsAndShape to get points and number of cards

	void ShowCard();																		 // Display cards, used for testing
	int CardCall(SuitStatistic &suitStatistic, int play /*,SUITANDRANK *previousCardCall*/); // Call cards
	int CallCardBegin(int play);															 // Start calling cards, starting from North

	SUITANDRANK GetSuitAndRankCall(int play, int cardNumber); // Get the suit and rank call for the current player
	int GetSuitAndRankCallPosition(int play);				  // Get the number of times the current player has called

	int SortCardSuitNum(SuitStatistic &suitStatistic, int order); // Sort the size of the suits
	void UserSuitCallCard(int play, int rankTon, int suitAmount);

	CardStack *GetCardStack(int player); // Get the card stack for the current player

	Card GetUserOutCard(int player);			// Get the card played by the player, determined by the parameter
	void SetUserOutCard(int player, Card card); // Record the card played by the user
	int CompareSizeOfOutCard();					// Compare the size of the 4 cards

private:
	ConsoleLib *m_console;
	Card m_playCard[PLAYNUM][PLAYCARDNUM]; // 4 players, each with 13 cards
	Card m_userOutCard[PLAYNUM];		   // Record the last card played

	SuitStatistic m_suitStatistic[PLAYNUM]; // Record the suit and points of the current player

	CardStack m_cardStack[PLAYNUM][SUITNUM]; // Record the 4 suits of each player, for display and calling

	SUITANDRANK m_suitAndRankCall[PLAYNUM][MAXCALLTONNUM]; // Record the number of calls made by each player
	int m_suitAndRankCallPosition[PLAYNUM];				   // Record the number of times each player has called

	SUITANDRANK m_previousCardCall[PLAYNUM]; // Record the cards called by players in the current round
};

#endif
