#pragma once
#ifndef _DECK_H_
#define _DECK_H_ 1
#pragma warning(disable : 4996)

#include "Play.h"
#include "GameMusicOperate.h"

typedef struct _TONANDSUIT // Ton and suit values
{
	char ton;
	char suit;
} TONANDSUIT;

typedef struct _SUITANDPOSITION // Suit and position
{
	int suit;
	int position;
} SUITANDPOSITION;

enum GameStatus
{
	// Game entry, game menu, game call card, game running, game over, game exit
	GameSplashScreen = 1,
	GameMainMenu = 2,
	GameCallCard = 3,
	GameRunning,
	GameOver,
	GameExit
};

enum SendCardStatus // Indicates whether the current card can be played
{
	CanSendCard = 1,
	NotSendCard = 7
};

class Deck
{
public:
	Deck();
	Deck(ConsoleLib *console, Play *play);
	virtual ~Deck();

	void SetConsoleLib(ConsoleLib *console); // Set the console
	void SetPlay(Play *play);				 // Set the game

	int GetStatus(void); // Get game status

	void ShowSplash(void); // Show game splash screen
	void ShowSplash(int x, int y);
	void ShowMenu(void);
	void ShowCall(void);
	void ShowGame(void);
	void ShowOver(void);
	void ShowExit(void);
	void Show(void); // Display corresponding game status

	void Process(void);
	void ProcessMenu(void);
	void ProcessCall(void);
	void ProcessGame(void);
	void ProcessOver(void);
	void ProcessExit(void);

	void StartCallGame(void);

	int GetAction(void);

	void DrawGameDesk();						 // Draw the game desk
	void DrawPlayerCardNorth(int cardNumber);	 // Draw North player's cards
	void DrawPlayerCardEast(int cardNumber);	 // Draw East player's cards
	void DrawPlayerCardSouth(int cardNumber);	 // Draw South player's cards
	void DrawPlayerCardWest(int cardNumber);	 // Draw West player's cards
	void DrawCallCardTonAndSuit();				 // Draw call card ton and suit
	void DrawUserOutCard();						 // Display user's played card
	COORD PlayerCardDisplayPosition(int player); // Display player's card position
	// void ShowCard(void);
	void DrawUserHintInformation(); // Display user hint information

	void DrawConventionTon(); // Draw final contract

	void CallCardTonAndSuitInit();							// Initialise call card
	int CallCardSuitSize(char suit);						// Calculate the size of each suit and NT
	void CallCardSuitSelect();								// Move call card up, down, left, and right
	void CallCardClearSelectCard(TONANDSUIT preTonAndSuit); // Clear selected card and display unselected card

	void CallCardUserCallDraw(); // Display user call record
	int SuitToInt(char suit);	 // Convert SUIT to enumeration type

	void SetpreviousUserCall(int play); // Replace the latest call, save the last call card

	int GetRemainderCardNumber(int player); // Get the remaining number of cards for the specified player

	Card UserSendOutCard();			 // User plays a card, determine the played card from the current selected card coordinates
	SendCardStatus UserCanOutCard(); // Determine if the user's selected card can be played based on suit
	void SendOutCardSetsOut();		 // Mark the current card as played

	void AISendOutCard(int player);
	int UserRotationSendCardNumber(int firstPlayer); // Start playing cards with the artificial player
	void AIFirstRotationOutCard();					 // The last card played in the first round after the game starts

	void UserOutCardSequence(int player); // Calculate the card playing sequence

	void PlayerWinAndLostReckon(int player);

private:
	Play *m_play;
	ConsoleLib *m_console;

	int m_lastAction; // Key press state

	int m_gameStatus; // Game status
	COORD m_arena;	  // Game area

	int m_selectGameMenu; // Selected game start menu

	COORD m_pilotSign; // Indicator coordinates, used to indicate the currently selected card

	TONANDSUIT m_tonAndSuit[8][5]; // Call cards, used to display calls

	COORD m_positionCallCard;	   // Position on the call card, indicating the call position
	COORD m_positionDisArr;		   // Corresponding position on the call card array, i.e., the position in the m_tonAndSuit array
	COORD m_borderDisposeCallCard; // Call card boundary handling

	TONANDSUIT m_previousTonAndSuit;		 // Record the previous call content of the current call card. If the first, it is 0, 0. Pass is not recorded. The call ends after 3 consecutive PASSes.
	TONANDSUIT m_CalLCardDrawLastTonAnDSuit; // Record the ton for which PASS D DD was not called, used to draw the call card

	int m_lineLastEmpty; // Record the last empty row

	bool m_inputFirstPos; // Record the current first position, used to mark the starting position

	int m_playerRemainderCards[PLAYNUM]; // Record the remaining number of cards for players

	SUITANDPOSITION m_suitPosition; // Record the suit and position of the played card

	int m_playerOutCardNumber; // Record the number of players who played cards

	bool m_userCardIsOrNotHint;	   // Check if the user's card matches the suit of the first player's card and provide a hint
	bool m_userThreeOutCardHint;   // Hint for player 3, i.e., the user playing a card
	int m_userFirstSentCardPlayer; // Record the first player who played a card

	int m_playerOutCardSort[PLAYNUM]; // Card playing sequence of players

	int m_playerWinAndLostRecord[PLAYNUM / 2]; // Record the win/loss count of players, 4 players, 2 pairs
};

#endif
