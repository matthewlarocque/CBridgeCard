#include <stdio.h>
#include <time.h>
#include "Deck.h"

GameMusicOperate *g_pGameMusicOperate; // BGM player instance

Deck::Deck()
{
}

Deck::Deck(ConsoleLib *console, Play *play)
{
	m_console = console;
	m_play = play;

	m_gameStatus = GameSplashScreen;
	m_lastAction = 0;

	m_pilotSign.X = 14; // Pilot sign head marker
	m_pilotSign.Y = 18;

	m_selectGameMenu = 1; // Game menu selection

	memset(m_tonAndSuit, 0, sizeof(TONANDSUIT) * 8 * 5);

	m_positionCallCard.X = 44; // Output position of the bid card
	m_positionCallCard.Y = 5;

	memset(&m_positionDisArr, 0, sizeof(COORD));
	memset(&m_previousTonAndSuit, 0, sizeof(m_previousTonAndSuit));

	m_lineLastEmpty = 0;

	m_inputFirstPos = true;

	CallCardTonAndSuitInit();

	memset(m_playerRemainderCards, 0, sizeof(int) * PLAYNUM); // Initialise

	for (int i = 0; i < PLAYNUM; i++)
	{
		m_playerRemainderCards[i] = 13;
	}

	srand((unsigned int)time(NULL)); // Set random variable seed

	m_playerOutCardNumber = 0; // Record how many players have played cards

	m_userCardIsOrNotHint = FALSE; // User card check whether the suit is the same and exist hint;

	m_userFirstSentCardPlayer = 1; // The first player to play a card

	m_userThreeOutCardHint = FALSE; // Hint whether player 3 has played a card

	memset(m_playerWinAndLostRecord, 0, sizeof(int) * PLAYNUM / 2); // Initialise player win/loss record
}

void Deck::CallCardTonAndSuitInit() // Initialise bid card values
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 5; j++)
		{
			m_tonAndSuit[i][j].ton = i + 1 + 48;
			if (0 == j)
				m_tonAndSuit[i][j].suit = char(5);
			if (1 == j)
				m_tonAndSuit[i][j].suit = char(4);
			if (2 == j)
				m_tonAndSuit[i][j].suit = char(3);
			if (3 == j)
				m_tonAndSuit[i][j].suit = char(6);
			if (4 == j)
				m_tonAndSuit[i][j].suit = 'N'; // NT
		}
	m_tonAndSuit[7][0].ton = 8 + 48; // Double
	m_tonAndSuit[7][0].suit = 'D';

	m_tonAndSuit[7][1].ton = 9 + 48; // Redouble
	m_tonAndSuit[7][1].suit = 'B';

	m_tonAndSuit[7][2].ton = 8 + 48; // PASS
	m_tonAndSuit[7][2].suit = 'P';

	m_tonAndSuit[7][3].ton = 0; // PASS
	m_tonAndSuit[7][3].suit = 0;
}

Deck::~Deck()
{
}

int Deck::GetStatus()
{
	return m_gameStatus;
}

void Deck::ShowSplash()
{
	COORD m_position;

	char *m_strPicture = "\n╭︿︿︿╮\n{/-●●-/}\n ( (oo) )\n  ︶︶︶ ";

	static int m_yPosit = 11;

	m_yPosit--; // Automatically move up, reduce the number of rows
	if (m_yPosit < -2)
	{
		m_yPosit = 11;
	}
	m_console->Clear();
	m_console->SetTextColor(ConRed | ConGreen | ConBlue);		 // Set the pig's colour to GREEN
	m_console->OutputString(m_strPicture, strlen(m_strPicture)); // Display the pig's picture

	m_console->SetTextColor(ConBlue | ConGreen); // Set text colour

	m_position.X = 18;
	m_position.Y = m_yPosit;
	if (m_position.Y > 0)
	{
		m_console->SetPosition(m_position);

		m_console->OutputString("The game is called Bridge, current version is 0.8");
	}
	else
		m_console->OutputString("");

	m_position.Y += 1;

	if (m_position.Y > 0)
	{
		m_console->SetPosition(m_position);

		m_console->OutputString("The game implements a simple bridge process!");
	}
	else
		m_console->OutputString("");

	m_position.Y += 1;

	if (m_position.Y > 0)
	{
		m_console->SetPosition(m_position);

		m_console->OutputString("The AI part is not satisfactory, players please bear with it!");
	}
	else
		m_console->OutputString("");

	m_position.Y += 1;
	if (m_position.Y > 0)
	{
		m_console->SetPosition(m_position);
	}
	else
		m_console->OutputString("");
	m_position.X = 57;
	m_position.Y = 23;
	m_console->SetPosition(m_position);									// Set coordinates
	m_console->SetTextColor(ConRed);									// Set text colour
	m_console->OutputString("-->Press ENTER to continue the game!<--"); // Output text
}

void Deck::ShowSplash(int x, int y)
{
	COORD m_position;

	m_console->Clear();
	m_console->SetTextColor(ConBlue | ConGreen);

	m_position.X = x;
	m_position.Y = y;
	if (m_position.Y > 0)
	{
		m_console->SetPosition(m_position);

		m_console->OutputString("The game is called Bridge! Current version is 0.8!");
	}
	else
		m_console->OutputString("");

	m_position.Y += 1;

	if (m_position.Y > 0)
	{
		m_console->SetPosition(m_position);

		m_console->OutputString("The game implements a simple bridge process!");
	}
	else
		m_console->OutputString("");

	m_position.Y += 1;

	if (m_position.Y > 0)
	{
		m_console->SetPosition(m_position);

		m_console->OutputString("The AI part is not satisfactory, leading to continuous bugs, players please bear with it!");
	}
	else
		m_console->OutputString("");
}

int Deck::GetAction()
{
	m_lastAction = m_console->GetKey();

	return m_lastAction;
}

void Deck::SetpreviousUserCall(int play) // Replacement handling of the latest bid
{
	SUITANDRANK m_callSuitAndRank = {0}; // Get the bid result
	m_callSuitAndRank = m_play->GetSuitAndRankCall(play, m_play->GetSuitAndRankCallPosition(play) - 1);

	// If the bid card is not PASS and the current point is greater than or equal to the previous point and the suit is greater than the previous suit, replace the previous value with the current value
	if (!m_callSuitAndRank.pass && (m_previousTonAndSuit.ton <= m_callSuitAndRank.rank + 48 || CallCardSuitSize(m_previousTonAndSuit.suit) < CallCardSuitSize(m_callSuitAndRank.suit)))
	{
		m_previousTonAndSuit.suit = m_callSuitAndRank.suit;
		m_previousTonAndSuit.ton = 48 + m_callSuitAndRank.rank;
	}
}

void Deck::StartCallGame()
{
	CardStatus m_callStatus = CallError;

	m_gameStatus = GameCallCard;
	m_arena.X = 79;
	m_arena.Y = 23;

	// Initialise cards, including dealing, shuffling, and point calculation
	m_play->SendCard();

	m_play->SortAndGetPlayerPAS(); // Sort the cards and get the corresponding points

	m_callStatus = (CardStatus)m_play->CallCardBegin(0);
	if (CallOver == m_callStatus) // Player 1 starts bidding
	{
		SetpreviousUserCall(0);
		m_gameStatus = GameRunning;
	}
	else if (CallAllPass == m_callStatus)
		m_gameStatus = GameExit;
	else
		SetpreviousUserCall(0);

	m_callStatus = (CardStatus)m_play->CallCardBegin(1);
	if (CallOver == m_callStatus) // Player 2 starts bidding
	{
		SetpreviousUserCall(1);
		m_gameStatus = GameRunning;
	}
	else if (CallAllPass == m_callStatus)
		m_gameStatus = GameExit;
	else
		SetpreviousUserCall(1);
}

void Deck::ProcessMenu(void)
{
	switch (m_lastAction)
	{
	case VK_ESCAPE:
	case 'Q': // Exit the game
	case 'q':
		m_gameStatus = GameExit;
		break;
	case '1':
		StartCallGame(); // Start bidding
		m_gameStatus = GameCallCard;
		break;
	case 'L':
	case 'l':

		break;
	case VK_UP: // Select game start menu
		m_selectGameMenu--;
		if (m_selectGameMenu <= 0)
			m_selectGameMenu = 3;
		break;
	case VK_DOWN:
		m_selectGameMenu++;
		if (m_selectGameMenu > 3)
			m_selectGameMenu = 1;
		break;
	case VK_SPACE:
		if (1 == m_selectGameMenu)
		{
			StartCallGame();
			m_gameStatus = GameCallCard;
		}
		else if (2 == m_selectGameMenu)
		{
		}
		else if (3 == m_selectGameMenu)
			m_gameStatus = GameExit;
		break;
	default:
		break;
	}
}

void Deck::AIFirstRotationOutCard()
{
	int m_surplusCardNumber = PLAYNUM - m_playerOutCardNumber; // Number of remaining players
	int m_surplusCardPostion = 0;							   // The corresponding positions of the remaining players

	if (1 == m_surplusCardNumber)
		m_surplusCardPostion = 4;
	else if (2 == m_surplusCardNumber)
		m_surplusCardPostion = 3;
	else if (3 == m_surplusCardNumber)
		m_surplusCardPostion = 2;

	for (int i = 0; i < m_surplusCardNumber; i++, m_surplusCardPostion++)
	{

		if (PLAYNUM < m_surplusCardPostion)
			m_surplusCardPostion = m_surplusCardPostion - 4;
		AISendOutCard(m_playerOutCardSort[m_surplusCardPostion - 1]); // Start playing cards
		DrawUserOutCard();
	}
}

void Deck::UserOutCardSequence(int player)
{
	switch (player)
	{
	case 1:
		m_playerOutCardSort[0] = 1;
		m_playerOutCardSort[1] = 2;
		m_playerOutCardSort[2] = 3;
		m_playerOutCardSort[3] = 4;
		break;
	case 2:
		m_playerOutCardSort[0] = 2;
		m_playerOutCardSort[1] = 3;
		m_playerOutCardSort[2] = 4;
		m_playerOutCardSort[3] = 1;
		break;
	case 3:
		m_playerOutCardSort[0] = 3;
		m_playerOutCardSort[1] = 4;
		m_playerOutCardSort[2] = 1;
		m_playerOutCardSort[3] = 2;
		break;
	case 4:
		m_playerOutCardSort[0] = 4;
		m_playerOutCardSort[1] = 1;
		m_playerOutCardSort[2] = 2;
		m_playerOutCardSort[3] = 3;
		break;
	default:
		break;
	}
}

void Deck::PlayerWinAndLostReckon(int player)
{
	switch (player)
	{
	case 1:
		m_playerWinAndLostRecord[0]++;
		break;
	case 2:
		m_playerWinAndLostRecord[1]++;
		break;
	case 3:
		m_playerWinAndLostRecord[0]++;
		break;
	case 4:
		m_playerWinAndLostRecord[1]++;
		break;
	default:
		break;
	}
}

void Deck::ProcessGame()
{
	switch (m_lastAction)
	{
	case VK_RIGHT:
		m_userCardIsOrNotHint = FALSE;
		m_userThreeOutCardHint = FALSE;

		m_pilotSign.X += 4;
		if (m_pilotSign.X >= GetRemainderCardNumber(3) * 4 + 13 + 1)
			m_pilotSign.X = 13 + 1;

		break;
	case VK_LEFT:
		m_userCardIsOrNotHint = FALSE;
		m_userThreeOutCardHint = FALSE;
		m_pilotSign.X -= 4;
		if (m_pilotSign.X < 14)											// Boundary handling
			m_pilotSign.X = GetRemainderCardNumber(3) * 4 + 13 + 1 - 4; // Get the remaining number of cards

		break;
	case VK_RETURN:
		if (m_playerOutCardNumber == UserRotationSendCardNumber(m_userFirstSentCardPlayer)) // Human player plays a card, i.e., player 3 plays a card
		{
			SendOutCardSetsOut();
			DrawUserOutCard();
		}

		if (!m_userCardIsOrNotHint || 4 == m_userFirstSentCardPlayer)
		{
			AIFirstRotationOutCard();

			if (4 == m_playerOutCardNumber)
			{
				DrawUserOutCard();
				Sleep(1000);
				m_userFirstSentCardPlayer = m_playerOutCardSort[m_play->CompareSizeOfOutCard()]; // The first player to play a card
				PlayerWinAndLostReckon(m_userFirstSentCardPlayer);								 // Calculate player win/loss situation and scores
				m_playerOutCardNumber = 0;														 // Reset card play
				m_playerOutCardSort[0] = 0;
				m_playerOutCardSort[1] = 0;
				m_playerOutCardSort[2] = 0;
				m_playerOutCardSort[3] = 0;
			}

			if (3 != m_userFirstSentCardPlayer)
			{
				UserOutCardSequence(m_userFirstSentCardPlayer); // User card play order

				for (int i = 0; i < PLAYNUM; i++)
				{
					if (3 == m_playerOutCardSort[i])
					{
						m_userThreeOutCardHint = TRUE; // Play card
						break;
					}
					if (0 == m_playerOutCardSort[i]) // Empty card, abnormal, exit
					{
						break;
					}
					AISendOutCard(m_playerOutCardSort[i]);
					DrawUserOutCard();
				}
			}
			else if (3 == m_userFirstSentCardPlayer)
			{
				// Please play a card
				m_userThreeOutCardHint = TRUE;					// Play card
				UserOutCardSequence(m_userFirstSentCardPlayer); // User card play order
			}
		}

		if (0 >= m_playerRemainderCards[0] && 0 >= m_playerRemainderCards[1] && 0 >= m_playerRemainderCards[2] && 0 >= m_playerRemainderCards[3])
		{
			m_gameStatus = GameOver;
			return;
		}
		break;
	}
}

void Deck::ProcessOver()
{
	if (VK_RETURN == m_lastAction)
		m_gameStatus = GameExit;
}

void Deck::ProcessExit()
{
}

void Deck::Process()
{
	static bool m_showRead = FALSE;

	static bool m_musicPlayer = FALSE;
	if (0 == m_musicPlayer)
	{
		g_pGameMusicOperate = new GameMusicOperate(); // Play BGM
		g_pGameMusicOperate->GetBackgroupMusic()->Play(300, TRUE);
		m_musicPlayer = TRUE;
	}

	if (0 == m_showRead)
	{

		if (GameSplashScreen == m_gameStatus)
			Show();
		Sleep(200);
		if (GetAsyncKeyState(VK_RETURN) < 0)
		{
			m_gameStatus = GameMainMenu;
			m_showRead = true;
		}
	}
	else
	{
		if (GetAction())
		{
			switch (m_gameStatus)
			{
			case GameMainMenu:
				ProcessMenu();
				break;
			case GameCallCard:
				ProcessCall();
				break;
			case GameRunning:
				ProcessGame();
				break;
			case GameOver:
				ProcessOver();
				break;
			case GameExit:
				ProcessExit();
				break;
			default:
				break;
			}
			Show();
		}
	}
}

void Deck::ShowMenu()
{
	COORD m_position;

	m_console->SetBackgroundColor(0);
	m_console->SetTextColor(ConRed);
	m_console->Clear();

	m_position.X = 33;
	m_position.Y = 10;
	m_console->SetPosition(m_position);
	if (1 == m_selectGameMenu)
		m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
	m_console->OutputString("1. Start Game");
	m_console->SetBackgroundColor(0);

	m_position.Y++;
	m_console->SetPosition(m_position);
	if (2 == m_selectGameMenu)
		m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
	m_console->OutputString("L. Load Game");
	m_console->SetBackgroundColor(0);

	m_position.Y++;
	m_console->SetPosition(m_position);
	if (3 == m_selectGameMenu)
		m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
	m_console->OutputString("Q. Quit Game");
	m_console->SetBackgroundColor(0);

	m_position.X = 57;
	m_position.Y = 22;
	m_console->SetPosition(m_position);									 // Set coordinates
	m_console->SetTextColor(ConRed | ConGreen);							 // Set text colour
	m_console->OutputString("--> Use arrow keys to select a game! <--"); // Output text

	m_position.X = 57;
	m_position.Y = 23;
	m_console->SetPosition(m_position);								 // Set coordinates
	m_console->SetTextColor(ConRed | ConBlue);						 // Set text colour
	m_console->OutputString("--> Press space to confirm game! <--"); // Output text
}

void Deck::DrawGameDesk()
{
	COORD m_position;

	m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
	m_console->SetTextColor(ConRed);

	for (int fieldY = 0; fieldY <= m_arena.Y; fieldY++) // Draw the border
	{
		if ((0 == fieldY) || (m_arena.Y == fieldY))
		{
			for (int fieldX = 0; fieldX <= m_arena.X; fieldX++) // Draw the horizontal table frame
			{
				m_position.X = fieldX;
				m_position.Y = fieldY;

				m_console->SetPosition(m_position);
				m_console->OutputString("-");
			}
		}
		else // Draw the vertical border
		{
			m_position.X = 0;
			m_position.Y = fieldY;
			m_console->SetPosition(m_position);
			m_console->OutputString("|");

			m_position.X = m_arena.X;
			m_console->SetPosition(m_position);
			m_console->OutputString("|");
		}
	}
}

void Deck::DrawPlayerCardNorth(int cardNumber)
{

	COORD m_position;

	m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
	m_console->SetTextColor(ConRed);
	m_position.Y = 3;

	for (int fieldX = 13; fieldX <= (PLAYCARDNUM + 3) * 4; fieldX += 4) // Draw the horizontal table frame
	{
		m_position.X = fieldX;
		m_console->SetPosition(m_position);
		m_console->OutputString(" *  ");
	}
	for (int m_space = 0; m_space < PLAYCARDNUM - cardNumber; m_space++) // Display the current number of cards
	{
		m_console->ClearChar(m_position.X, m_position.Y);
		m_console->ClearChar(m_position.X + 1, m_position.Y);
		m_console->ClearChar(m_position.X + 2, m_position.Y);
		m_console->ClearChar(m_position.X + 3, m_position.Y);
		m_position.X -= 4;
	}
}

void Deck::DrawPlayerCardWest(int cardNumber)
{

	COORD m_position;

	m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
	m_console->SetTextColor(ConRed);
	m_position.X = 13;

	for (int fieldY = 4; fieldY <= PLAYCARDNUM + 3; fieldY++) // Draw the border
	{
		m_position.Y = fieldY;
		m_console->SetPosition(m_position);
		m_console->OutputString("*");
	}
	for (int m_space = 0; m_space < PLAYCARDNUM - cardNumber; m_space++) // Display the current number of cards
	{
		m_console->ClearChar(m_position.X, m_position.Y);
		m_position.Y -= 1;
	}
}

void Deck::DrawPlayerCardEast(int cardNumber)
{

	COORD m_position;

	m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
	m_console->SetTextColor(ConRed);
	m_position.X = 64;

	for (int fieldY = 4; fieldY <= PLAYCARDNUM + 3; fieldY++) // Draw the border
	{
		m_position.Y = fieldY;

		m_console->SetPosition(m_position);
		m_console->OutputString("*");
	}
	for (int m_space = 0; m_space < PLAYCARDNUM - cardNumber; m_space++) // Display the current number of cards
	{
		m_console->ClearChar(m_position.X, m_position.Y);
		m_position.Y -= 1;
	}
}

int Deck::GetRemainderCardNumber(int player)
{
	CardStack *m_cardAllSuit = m_play->GetCardStack(player); // Get the cards of the specified player
	int m_count = 0;										 // Number of cards

	for (int m_suitNum = 0; m_suitNum < SUITNUM; m_suitNum++) // Number of suits, 4 suits
	{
		m_count += m_cardAllSuit[m_suitNum].GetStackSize() - m_cardAllSuit[m_suitNum].GetSendCardNum();
	}

	return m_count;
}

Card Deck::UserSendOutCard()
{
	int m_sentCardPos = -1;			  // Current card position
	int m_suitNumber[SUITNUM] = {-1}; // Record the remaining quantity of each suit
	Card m_sendCard;

	memset(&m_sendCard, 0, sizeof(Card));

	CardStack *m_cardAllSuit = m_play->GetCardStack(3);

	m_sentCardPos = (m_pilotSign.X - 13 - 1) / 4 + 1;									   // Card position
	m_suitNumber[0] = m_cardAllSuit[0].GetStackSize() - m_cardAllSuit[0].GetSendCardNum(); // Number of spades remaining
	m_suitNumber[1] = m_cardAllSuit[1].GetStackSize() - m_cardAllSuit[1].GetSendCardNum(); // Number of hearts remaining
	m_suitNumber[2] = m_cardAllSuit[2].GetStackSize() - m_cardAllSuit[2].GetSendCardNum(); // Number of diamonds remaining
	m_suitNumber[3] = m_cardAllSuit[3].GetStackSize() - m_cardAllSuit[3].GetSendCardNum(); // Number of clubs remaining

	if (m_suitNumber[0] > 0 && m_sentCardPos <= m_suitNumber[0]) // Selected card is in the spades area
	{
		for (int i = 0, k = 0; i < m_cardAllSuit[0].GetStackSize(); i++)
		{
			if (FALSE == m_cardAllSuit[0].GetSendCardStaus(i + 1)) // Find a card that has not been played
				k++;
			if (k == m_sentCardPos) // Found card is the selected card
			{
				m_suitPosition.suit = 0; // Record suit and position, mark this card as played
				m_suitPosition.position = i + 1;

				memcpy(&m_sendCard, &m_cardAllSuit[0].GetVauleFigure(i + 1), sizeof(m_sendCard));
				break;
			}
		}
	}
	else if (m_sentCardPos > m_suitNumber[0] && m_sentCardPos <= (m_suitNumber[0] + m_suitNumber[1]))
	{
		for (int i = 0, k = m_suitNumber[0]; i < m_cardAllSuit[1].GetStackSize(); i++)
		{
			if (FALSE == m_cardAllSuit[1].GetSendCardStaus(i + 1))
				k++;
			if (k == m_sentCardPos)
			{
				m_suitPosition.suit = 1;
				m_suitPosition.position = i + 1;

				memcpy(&m_sendCard, &m_cardAllSuit[1].GetVauleFigure(i + 1), sizeof(m_sendCard));
				break;
			}
		}
	}
	else if ((m_sentCardPos > (m_suitNumber[0] + m_suitNumber[1])) && (m_sentCardPos <= (m_suitNumber[0] + m_suitNumber[1] + m_suitNumber[2])))
	{
		for (int i = 0, k = m_suitNumber[0] + m_suitNumber[1]; i < m_cardAllSuit[2].GetStackSize(); i++)
		{
			if (FALSE == m_cardAllSuit[2].GetSendCardStaus(i + 1))
				k++;
			if (k == m_sentCardPos)
			{
				m_suitPosition.suit = 2;
				m_suitPosition.position = i + 1;

				memcpy(&m_sendCard, &m_cardAllSuit[2].GetVauleFigure(i + 1), sizeof(m_sendCard));
				break;
			}
		}
	}
	else if ((m_sentCardPos > (m_suitNumber[0] + m_suitNumber[1] + m_suitNumber[2])) && (m_sentCardPos <= (m_suitNumber[0] + m_suitNumber[1] + m_suitNumber[2] + m_suitNumber[3])))
	{
		for (int i = 0, k = (m_suitNumber[0] + m_suitNumber[1] + m_suitNumber[2]); i < m_cardAllSuit[3].GetStackSize(); i++)
		{
			if (FALSE == m_cardAllSuit[3].GetSendCardStaus(i + 1))
				k++;
			if (k == m_sentCardPos)
			{
				m_suitPosition.suit = 3;
				m_suitPosition.position = i + 1;

				memcpy(&m_sendCard, &m_cardAllSuit[3].GetVauleFigure(i + 1), sizeof(m_sendCard));
				break;
			}
		}
	}

	return m_sendCard;
}
SendCardStatus Deck::UserCanOutCard()
{
	Card m_sendOutCard = UserSendOutCard(); // Get the card selected by the player

	Card m_outCard = m_play->GetUserOutCard(1); // The first player to play, the suit of the subsequent cards will be based on this player's suit
	CardStack *m_cardAllSuit = m_play->GetCardStack(3);
	int m_suitGet = -1; // Get the suit size

	if ((m_sendOutCard.GetSuit() == m_outCard.GetSuit()) || 3 == m_userFirstSentCardPlayer) // Same suit, can play
	{
		return CanSendCard;
	}
	else // Different suit, check if the current suit is already played out
	{
		m_suitGet = 4 - CallCardSuitSize(m_outCard.GetSuit()); // Get the suit size, spades are 4
		if (0 != (m_cardAllSuit[m_suitGet].GetStackSize() - m_cardAllSuit[m_suitGet].GetSendCardNum()))
			return NotSendCard;
		else
			return CanSendCard;
	}
}

void Deck::SendOutCardSetsOut()
{
	CardStack *m_cardAllSuit = m_play->GetCardStack(3);
	COORD m_position = {30, 15};

	if (CanSendCard == UserCanOutCard())
	{
		m_pilotSign.X -= 4;
		if (m_pilotSign.X < 14) // Boundary handling
			m_pilotSign.X = 13 + 1;

		m_playerOutCardNumber++;
		m_cardAllSuit[m_suitPosition.suit].SetSendCardStaue(m_suitPosition.position, CardSendStaus);
		m_playerRemainderCards[3 - 1]--; // Array index starts from 0, the third player
		m_play->SetUserOutCard(m_playerOutCardNumber, m_cardAllSuit[m_suitPosition.suit].GetVauleFigure(m_suitPosition.position));
		// Point to the next element
	}
	else
		m_userCardIsOrNotHint = TRUE;
}

void Deck::AISendOutCard(int player)
{
	Card m_outCard = m_play->GetUserOutCard(1); // Get the first card played
	int m_suitGet = 0;							// Suit
	CardStack *m_cardAllSuit = m_play->GetCardStack(player);
	int m_cardGet = 0;		// Random card
	int m_cardSuitRand = 0; // Random suit

	if ((0 == m_outCard.GetSuit() && 0 == m_outCard.GetRank()) && 0 == m_outCard.GetSelect()) // If both suit and rank are 0 and Select is also 0, it means this player is the first player in the entire game
	{
		m_playerOutCardNumber++;

		m_suitGet = rand() % SUITNUM;

		memcpy(&m_outCard, &m_cardAllSuit[m_suitGet].GetVauleFigure(1), sizeof(Card));
		m_play->SetUserOutCard(m_playerOutCardNumber, m_outCard);
		m_cardAllSuit[m_suitGet].SetSendCardStaue(1, CardSendStaus);
		m_playerRemainderCards[player - 1]--;
	}
	else
	{
		if (0 == m_outCard.GetSuit())
			m_suitGet = rand() % SUITNUM;
		else
			m_suitGet = 4 - CallCardSuitSize(m_outCard.GetSuit()); // Get the suit size, spades are 4, get the suit of the first card played
		m_playerOutCardNumber++;

		if (0 < (m_cardAllSuit[m_suitGet].GetStackSize() - m_cardAllSuit[m_suitGet].GetSendCardNum()))
		{
			do
			{
				m_cardGet = rand() % m_cardAllSuit[m_suitGet].GetStackSize() + 1; // Randomly play a card
			} while (CardNotSendStaus != m_cardAllSuit[m_suitGet].GetSendCardStaus(m_cardGet) && 0 < m_playerRemainderCards[player - 1]);
			memcpy(&m_outCard, &m_cardAllSuit[m_suitGet].GetVauleFigure(m_cardGet), sizeof(Card)); // Assign value
			m_play->SetUserOutCard(m_playerOutCardNumber, m_outCard);							   // Record the card played by this user
			m_cardAllSuit[m_suitGet].SetSendCardStaue(m_cardGet, CardSendStaus);				   // This card is played
			m_playerRemainderCards[player - 1]--;												   // The number of cards for this user is reduced by one
		}
		else
		{
			do
			{
				m_cardSuitRand = rand() % SUITNUM; // Randomly play a suit
			} while (((m_suitGet == m_cardSuitRand) || (0 >= (m_cardAllSuit[m_cardSuitRand].GetStackSize() - m_cardAllSuit[m_cardSuitRand].GetSendCardNum()))) && 0 < m_playerRemainderCards[player - 1]);
			do
			{
				if (m_cardAllSuit[m_cardSuitRand].GetStackSize())
					m_cardGet = rand() % m_cardAllSuit[m_cardSuitRand].GetStackSize() + 1; // Randomly play a card
			} while ((CardNotSendStaus != m_cardAllSuit[m_cardSuitRand].GetSendCardStaus(m_cardGet)) && 0 < m_playerRemainderCards[player - 1]);

			memcpy(&m_outCard, &m_cardAllSuit[m_cardSuitRand].GetVauleFigure(m_cardGet), sizeof(Card)); // Assign value
			m_play->SetUserOutCard(m_playerOutCardNumber, m_outCard);									// Record the card played by this user
			m_cardAllSuit[m_cardSuitRand].SetSendCardStaue(m_cardGet, CardSendStaus);					// This card is played
			m_playerRemainderCards[player - 1]--;														// The number of cards for this user is reduced by one
		}
	}
}

void Deck::DrawPlayerCardSouth(int cardNumber)
{
	CardStack *m_cardAllSuit = m_play->GetCardStack(3); // Get the cards of the third player as the current player's cards

	COORD m_position;
	// test
	m_position.Y = 17;

	char m_suit = 0; // Get the suit
	int m_rank = 0;	 // Get the rank
	char m_rankIntToChar[3] = {0};
	int fieldX = 13;

	for (int m_suitNum = 0; m_suitNum < SUITNUM; m_suitNum++)											 // Number of suits, 4 suits
		for (int m_count = 0; m_count < m_cardAllSuit[m_suitNum].GetStackSize(); fieldX += 4, m_count++) // Draw the horizontal table frame
		{
			if (CardSendStaus == m_cardAllSuit[m_suitNum].GetSendCardStaus(m_count + 1))
			{
				fieldX -= 4; // This card is already played, revert to the previous state, ignore this card, each card occupies 4 spaces
				continue;
			}
			m_position.X = fieldX;
			m_console->SetPosition(m_position);

			if (0 == m_suitNum || 3 == m_suitNum) // Spades and Clubs, font colour is black
				m_console->SetTextColor(0);
			else
				m_console->SetTextColor(ConRed); // Other fonts are red
			m_suit = m_cardAllSuit[m_suitNum].GetCard(m_count).GetSuit();
			m_console->OutputString(" ");
			m_console->OutputString(&m_suit, 1);

			switch (m_cardAllSuit[m_suitNum].GetCard(m_count).GetRank())
			{
			case 1:
				sprintf(m_rankIntToChar, "%c", 'A');
				break;
			case 10:
				sprintf(m_rankIntToChar, "%c", 'T');
				break;
			case 11:
				sprintf(m_rankIntToChar, "%c", 'J');
				break;
			case 12:
				sprintf(m_rankIntToChar, "%c", 'Q');
				break;
			case 13:
				sprintf(m_rankIntToChar, "%c", 'K');
				break;
			default:
				sprintf(m_rankIntToChar, "%d", m_cardAllSuit[m_suitNum].GetCard(m_count).GetRank());
				break;
			}

			m_console->OutputString(m_rankIntToChar, strlen(m_rankIntToChar));
			m_console->OutputString(" ");
		}
	// Draw the table: the cards that have been played, draw as empty
	for (int m_suitNum = 0; m_suitNum < SUITNUM; m_suitNum++)
		for (int i = 0; i < m_cardAllSuit[m_suitNum].GetSendCardNum(); i++)
		{
			m_position.X += 4;
			m_console->SetPosition(m_position);
			m_console->OutputString("    ");
		}
	char m_sign = char(30);
	m_console->SetBackgroundColor(0);
	m_console->SetTextColor(ConRed);
	m_console->SetPosition(m_pilotSign);
	m_console->OutputString(&m_sign, 1);
}

void Deck::DrawCallCardTonAndSuit() // Drawing of the bid card
{
	COORD m_position;
	m_position.Y = 5;
	m_position.X = 44;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (0 != m_tonAndSuit[i][j].ton && 0 != m_tonAndSuit[i][j].suit)
			{
				m_console->SetPosition(m_position);
				m_console->OutputString(&m_tonAndSuit[i][j].ton, 1);
				m_console->OutputString(&m_tonAndSuit[i][j].suit, 1);
				m_console->OutputString(" ");
				m_position.X += 3;
			}
		}
		m_position.X = 44;
		m_position.Y += 1;
	}

	m_console->SetPosition(m_positionCallCard);											   // Current position of the selected card on the bid card
	m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);							   // Background colour of the selected card
	m_console->OutputString(&m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].ton, 1); // Output the selected card
	m_console->OutputString(&m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].suit, 1);
}

void Deck::CallCardUserCallDraw()
{
	COORD m_callUseDrawPos = {0};
	char m_playName = 0;
	static int m_disNumber = 0;

	m_callUseDrawPos.Y = 5;
	m_callUseDrawPos.X = 26;
	m_console->SetBackgroundColor(ConBlue | ConGreen);
	for (int k = 0; k < PLAYNUM; k++)
	{
		if (0 == k)
			m_playName = 'N';
		else if (1 == k)
			m_playName = 'E';
		else if (2 == k)
			m_playName = 'S';
		else if (3 == k)
			m_playName = 'W';
		m_console->SetPosition(m_callUseDrawPos);
		m_console->OutputString(" ");
		m_console->OutputString(&m_playName, 1);
		m_console->OutputString(" ");
		m_callUseDrawPos.X += 3;
	}
	m_callUseDrawPos.Y = 6;
	m_callUseDrawPos.X = 26;

	m_console->SetTextColor(ConGreen | ConBlue | ConRed);
	m_console->SetBackgroundColor(0);

	for (int i = 0; i < PLAYNUM; i++)
	{
		for (int j = m_disNumber; j < m_play->GetSuitAndRankCallPosition(i); j++)
		{
			SUITANDRANK m_callSuitAndRank = m_play->GetSuitAndRankCall(i, j);

			m_console->SetPosition(m_callUseDrawPos);

			if (true == m_callSuitAndRank.pass)
			{
				m_console->OutputString(" ");
				m_console->OutputString("P");
				m_console->OutputString(" ");
			}
			else
			{
				char m_intToChar = m_callSuitAndRank.rank + 48;
				m_console->OutputString(&m_intToChar, 1);
				m_console->OutputString(&m_callSuitAndRank.suit, 1);
				m_console->OutputString(" ");
			}
			m_callUseDrawPos.Y++;
			if (m_callUseDrawPos.Y > 15) // Exceeding the boundary, the last row displays the latest
			{
				m_callUseDrawPos.Y = 16;
				m_disNumber++;
				Show(); // Redraw the display panel
			}
		}
		m_callUseDrawPos.X += 3;
		if (m_callUseDrawPos.X > 38)
		{
			m_callUseDrawPos.X = 26;
		}
		m_callUseDrawPos.Y = 6;
	}
}

void Deck::ShowCall()
{
	DrawGameDesk();			 // Draw the game desk
	DrawPlayerCardNorth(13); // Draw the cards of player North
	DrawPlayerCardEast(13);	 // Draw the cards of player East
	DrawPlayerCardWest(13);	 // Draw the cards of player West

	DrawPlayerCardSouth(13);  // Draw the cards of player South
	DrawCallCardTonAndSuit(); // Draw the bid card

	CallCardClearSelectCard(m_previousTonAndSuit); // Clear the cards before the called cards, i.e., the current card must be higher than the previously called card
	CallCardUserCallDraw();						   // Output the user's bid situation
}

void Deck::DrawConventionTon() // Draw the final contract
{
	COORD m_position = {0};
	m_position.Y = 2;
	m_position.X = 2;

	m_console->SetBackgroundColor(ConBlue | ConGreen); // Set background colour
	m_console->SetTextColor(0);						   // Set text colour to black

	m_console->SetPosition(m_position); // Set the output position
	m_console->OutputString("Contract:");
	m_console->OutputString(&m_previousTonAndSuit.ton, 1);	// Output the number of tricks
	m_console->OutputString(&m_previousTonAndSuit.suit, 1); // Output the suit
}

COORD Deck::PlayerCardDisplayPosition(int player)
{
	COORD m_position = {0};
	switch (player)
	{
	case 1:
		m_position.X = 36;
		m_position.Y = 8;
		break;
	case 2:
		m_position.X = 37;
		m_position.Y = 9;
		break;
	case 3:
		m_position.X = 36;
		m_position.Y = 10;
		break;
	case 4:
		m_position.X = 35;
		m_position.Y = 9;
		break;
	default:
		break;
	}
	return m_position;
}
int Deck::UserRotationSendCardNumber(int firstPlayer)
{
	switch (firstPlayer)
	{
	case 1:
		return 2;
	case 2:
		return 1;
	case 3:
		return 0;
	case 4:
		return 3;
	default:
		return 0;
	}
}

void Deck::DrawUserOutCard()
{
	COORD m_position = {0};
	char m_suit = 0;
	char m_rankIntToChar[3] = {0}; // INT to CHAR

	int m_player = 0;
	if (0 != m_play->GetUserOutCard(1).GetSuit()) // Data exists, display the data
	{
		for (int i = 0; i < PLAYNUM; i++)
		{
			m_player = i + m_userFirstSentCardPlayer;
			if (m_player > 4)
				m_player = m_player - 4;
			m_position = PlayerCardDisplayPosition(m_player);
			m_console->SetPosition(m_position);
			m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);
			m_suit = m_play->GetUserOutCard(i + 1).GetSuit();

			if (0 == m_suit)
				break;
			if (char(6) == m_suit || char(5) == m_suit) // Spades and Clubs, font colour is black
				m_console->SetTextColor(0);
			else
				m_console->SetTextColor(ConRed); // Other fonts are red

			m_console->OutputString(&(m_suit), 1);

			switch (m_play->GetUserOutCard(i + 1).GetRank())
			{
			case 1:
				sprintf(m_rankIntToChar, "%c", 'A');
				break;
			case 10:
				sprintf(m_rankIntToChar, "%c", 'T');
				break;
			case 11:
				sprintf(m_rankIntToChar, "%c", 'J');
				break;
			case 12:
				sprintf(m_rankIntToChar, "%c", 'Q');
				break;
			case 13:
				sprintf(m_rankIntToChar, "%c", 'K');
				break;
			default:
				sprintf(m_rankIntToChar, "%d", m_play->GetUserOutCard(i + 1).GetRank());
				break;
			}

			m_console->OutputString(m_rankIntToChar, strlen(m_rankIntToChar));
		}
	}
	m_console->SetBackgroundColor(0);
}

void Deck::DrawUserHintInformation()
{
	COORD m_position = {23, 15};
	if (m_userCardIsOrNotHint)
	{
		m_console->SetPosition(m_position);
		m_console->SetBackgroundColor(0);
		m_console->SetTextColor(ConRed);
		m_console->OutputString("There are remaining cards of this suit, please play again!");
	}
	if (m_userThreeOutCardHint)
	{
		m_position.X = 10;
		m_position.Y = 20;
		m_console->SetPosition(m_position);
		m_console->SetBackgroundColor(0);
		m_console->SetTextColor(ConRed);
		m_console->OutputString("Please play a card:");
	}
}

void Deck::ShowGame()
{
	DrawGameDesk(); // Draw the game desk
	DrawConventionTon();

	DrawPlayerCardNorth(m_playerRemainderCards[0]); // Draw the cards of player North
	DrawPlayerCardEast(m_playerRemainderCards[1]);	// Draw the cards of player East
	DrawPlayerCardWest(m_playerRemainderCards[3]);	// Draw the cards of player West

	DrawPlayerCardSouth(m_playerRemainderCards[2]); // Draw the cards of player South

	DrawUserOutCard(); // Draw the user's card play record

	DrawUserHintInformation(); // Draw the user's card play hint information
}

int Deck::CallCardSuitSize(char suit) // Order of suits and NT in bidding
{
	int m_suitSize = 0;

	switch (suit)
	{
	case char(5): // Clubs are the smallest
		m_suitSize = 1;
		break;
	case char(4):
		m_suitSize = 2;
		break;
	case char(3):
		m_suitSize = 3;
		break;
	case char(6):
		m_suitSize = 4;
		break;
	case 'N':
		m_suitSize = 5;
		break;
	default:
		break;
	}
	return m_suitSize;
}

void Deck::CallCardClearSelectCard(TONANDSUIT preTonAndSuit) // Handle bid cards, remove bids lower than the current one
{
	COORD m_tonAndSuitPos;

	if (('8' == preTonAndSuit.ton && 'D' == preTonAndSuit.suit) // Double and PASS, do not change the drawing of the bid card
		|| ('9' == preTonAndSuit.ton && 'B' == preTonAndSuit.suit) || ('8' == preTonAndSuit.ton && 'P' == preTonAndSuit.suit))
	{
	}
	else
	{
		m_CalLCardDrawLastTonAnDSuit.ton = preTonAndSuit.ton;
		m_CalLCardDrawLastTonAnDSuit.suit = preTonAndSuit.suit;
	}

	preTonAndSuit.ton = m_CalLCardDrawLastTonAnDSuit.ton;
	preTonAndSuit.suit = m_CalLCardDrawLastTonAnDSuit.suit;

	m_tonAndSuitPos.X = 44;
	m_tonAndSuitPos.Y = 5;
	for (int i = 0; i < preTonAndSuit.ton - 1 - 48; i++) // Handle the number of rows
	{
		for (int k = 0; k < 5; k++) // Each row has 5 values, all are empty
		{
			m_console->SetPosition(m_tonAndSuitPos);
			m_console->SetBackgroundColor(0);
			m_console->OutputString("   ");
			m_tonAndSuitPos.X += 3;
			if (15 + 44 <= m_tonAndSuitPos.X) // Exceeding the boundary, start from the beginning
				m_tonAndSuitPos.X = 44;
		}
		m_tonAndSuitPos.Y += 1;
		if (7 + 5 < m_tonAndSuitPos.Y) // A total of 7 rows, starting position from 5, exceeding 7 rows, the last row is a special option, do not handle
			m_tonAndSuitPos.Y = 7;
	}

	for (int j = 0; j < CallCardSuitSize(preTonAndSuit.suit); j++) // Incomplete row handling, represented by the number of values to be handled by the size of the suit
	{
		m_console->SetPosition(m_tonAndSuitPos);
		m_console->SetBackgroundColor(0);
		m_console->OutputString("   ");
		m_tonAndSuitPos.X += 3;
		if (15 + 44 <= m_tonAndSuitPos.X)
		{
			m_tonAndSuitPos.X = 44;
			m_tonAndSuitPos.Y++;
		}
	}
	if (12 == m_tonAndSuitPos.Y) // Last row
	{
		m_inputFirstPos = false;
	}

	m_console->SetBackgroundColor(ConBlue | ConGreen | ConRed);

	m_borderDisposeCallCard.X = m_tonAndSuitPos.X;
	m_borderDisposeCallCard.Y = m_tonAndSuitPos.Y;

	if (m_inputFirstPos) // Mark the starting position
	{
		if (7 == m_positionCallCard.Y - 5)
		{
			m_console->SetPosition(m_positionCallCard);																  // Current position of the selected card on the bid card
			m_console->SetBackgroundColor(0);																		  // Background colour of the selected card
			m_console->OutputString(&m_tonAndSuit[m_positionCallCard.Y - 5][(m_positionCallCard.X - 44) / 3].ton, 1); // Output the selected card
			m_console->OutputString(&m_tonAndSuit[m_positionCallCard.Y - 5][(m_positionCallCard.X - 44) / 3].suit, 1);
		}

		m_positionCallCard.X = m_borderDisposeCallCard.X;
		m_positionCallCard.Y = m_borderDisposeCallCard.Y;

		m_positionDisArr.X = (m_borderDisposeCallCard.X - 44) / 3;
		m_positionDisArr.Y = m_borderDisposeCallCard.Y - 5;

		m_console->SetPosition(m_borderDisposeCallCard);													// Current position of the selected card on the bid card
		m_console->SetBackgroundColor(ConRed | ConGreen | ConBlue);											// Background colour of the selected card
		m_console->OutputString(&m_tonAndSuit[m_tonAndSuitPos.Y - 5][(m_tonAndSuitPos.X - 44) / 3].ton, 1); // Output the selected card
		m_console->OutputString(&m_tonAndSuit[m_tonAndSuitPos.Y - 5][(m_tonAndSuitPos.X - 44) / 3].suit, 1);
	}
}

int Deck::SuitToInt(char suit)
{
	switch (suit) // Return the corresponding suit quantity, determined by order, 0 is the most suits
	{
	case char(6):
		return Spade;
	case char(3):
		return Heart;
	case char(4):
		return Diamond;
	case char(5):
		return Club;
	case 'N':
		return SuitNT;
	case 'D':
		return Double;
	case 'B':
		return DDouble;
	case 'P':
		return Pass;
	default:
		return SuitError;
	}
}

void Deck::CallCardSuitSelect()
{
	CardStatus m_callStatus = CallError;
	switch (m_lastAction)
	{
	case VK_RIGHT:
		m_positionCallCard.X += 3;
		if (m_positionCallCard.X > 44 + 12) // Boundary, 5 columns, each occupying 3 columns
			m_positionCallCard.X = 44 + 12;

		m_positionDisArr.X++;
		if (m_positionDisArr.X > 4) // Boundary check
			m_positionDisArr.X = 4;
		if (7 == m_positionDisArr.Y) // Last row, only 3 options
		{
			if (m_positionDisArr.X > 2)
				m_positionDisArr.X = 2;
			if (m_positionCallCard.X > 44 + 6)
				m_positionCallCard.X = 44 + 6;
		}

		m_inputFirstPos = false;
		break;

	case VK_LEFT:
		m_positionCallCard.X -= 3;	   // Move 3 columns each time
		if (m_positionCallCard.X < 44) // Left boundary
			m_positionCallCard.X = 44;
		m_positionDisArr.X--;
		if (m_positionDisArr.X < 0) // Left boundary
			m_positionDisArr.X = 0;
		if (m_previousTonAndSuit.ton - 48 == m_positionCallCard.Y - 4) // First row
		{
			if (m_positionDisArr.X < (CallCardSuitSize(m_previousTonAndSuit.suit))) // Boundary handling
				m_positionDisArr.X = (CallCardSuitSize(m_previousTonAndSuit.suit));
			if (m_positionCallCard.X < 44 + (CallCardSuitSize(m_previousTonAndSuit.suit)) * 3)
				m_positionCallCard.X = 44 + (CallCardSuitSize(m_previousTonAndSuit.suit)) * 3;
		}

		m_inputFirstPos = false;
		break;

	case VK_DOWN:
		m_positionCallCard.Y += 1;
		if (m_positionCallCard.Y > 5 + 7) // Start from the 5th row, a total of 8 rows
			m_positionCallCard.Y = 5 + 7;
		m_positionDisArr.Y++;
		if (m_positionDisArr.Y > 7)
			m_positionDisArr.Y = 7;

		if (7 == m_positionDisArr.Y) // Last row, only 3 data in the last row
		{
			if (m_positionDisArr.X > 2)
				m_positionDisArr.X = 2;
			if (m_positionCallCard.X > 44 + 6)
				m_positionCallCard.X = 44 + 6;
		}

		m_inputFirstPos = false;
		break;
	case VK_UP:
		m_positionCallCard.Y -= 1;	  // Card position
		if (m_positionCallCard.Y < 5) // Boundary, first row
			m_positionCallCard.Y = 5;
		m_positionDisArr.Y--; // Card array coordinates
		if (m_positionDisArr.Y < 0)
			m_positionDisArr.Y = 0;

		if (m_positionDisArr.Y <= m_borderDisposeCallCard.Y - 5) // Bid card boundary handling, starting from the fifth row, the bid card coordinate corresponding to the fifth row is 0
		{
			m_positionCallCard.X = m_borderDisposeCallCard.X;
			m_positionCallCard.Y = m_borderDisposeCallCard.Y;

			m_positionDisArr.X = (m_borderDisposeCallCard.X - 44) / 3;
			m_positionDisArr.Y = m_borderDisposeCallCard.Y - 5;
		}
		if (m_positionDisArr.Y < m_lineLastEmpty)
		{
			m_positionCallCard.Y++;
			m_positionDisArr.Y++;
		}

		m_inputFirstPos = false;
		break;

	case VK_RETURN:

		// Output selected card
		if ('D' != m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].suit && 'B' != m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].suit && 'P' != m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].suit)
		{
			m_previousTonAndSuit.ton = m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].ton;
			m_previousTonAndSuit.suit = m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].suit;
		}
		m_play->UserSuitCallCard(3, m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].ton - 48, SuitToInt(m_tonAndSuit[m_positionDisArr.Y][m_positionDisArr.X].suit)); // Player 3, corresponding array coordinates are 2 (array coordinates start from 0), already subtracted in this function, so pass in 3 instead of 2

		m_callStatus = (CardStatus)m_play->CallCardBegin(3);

		if (0 == m_previousTonAndSuit.ton)
		{
			m_gameStatus = GameExit;
			return;
		}

		if (CallOver == m_callStatus) // Player 4 starts bidding
		{
			SetpreviousUserCall(3);
			m_gameStatus = GameRunning;
		}
		else if (CallAllPass == m_callStatus)
			m_gameStatus = GameExit;
		else
			SetpreviousUserCall(3);

		m_callStatus = (CardStatus)m_play->CallCardBegin(0);
		if (CallOver == m_callStatus) // Player 1 starts bidding
		{
			SetpreviousUserCall(0);
			m_gameStatus = GameRunning;
		}
		else if (CallAllPass == m_callStatus)
			m_gameStatus = GameExit;
		else
			SetpreviousUserCall(0);

		m_callStatus = (CardStatus)m_play->CallCardBegin(1);

		if (CallOver == m_callStatus) // Player 2 starts bidding
		{
			SetpreviousUserCall(1);
			m_gameStatus = GameRunning;
		}
		else if (CallAllPass == m_callStatus)
			m_gameStatus = GameExit;
		else
			SetpreviousUserCall(1);

		if (56 == m_positionCallCard.X && 4 == m_positionDisArr.X) // Last element in horizontal, switch to next row
		{
			m_positionDisArr.X = 0;
			m_positionDisArr.Y++;
			m_positionCallCard.Y++;
			m_positionCallCard.X = 44;
			m_lineLastEmpty = m_positionDisArr.Y;
		}
		else
		{
			if (7 != m_positionDisArr.Y) // Not the last data
			{
				m_positionDisArr.X++;
				m_positionCallCard.X += 3;
			}
		}
		m_inputFirstPos = true;
		if (GameRunning == m_gameStatus)
		{
			for (int i = 0; i < PLAYNUM; i++) // Playing order 1,2,3,4
				m_playerOutCardSort[i] = i + 1;

			for (int i = 0; i < PLAYNUM; i++)
			{
				if (3 == m_playerOutCardSort[i])
				{
					m_userThreeOutCardHint = TRUE; // Play card
					break;
				}
				AISendOutCard(m_playerOutCardSort[i]);
			}
		}

		break;
	default:
		break;
	}
}

void Deck::ProcessCall()
{
	CallCardSuitSelect(); // Select a bidding item on the bid card
}

void Deck::ShowOver()
{

	COORD m_position = {0}; // Position
	char m_buffer[100] = {0};

	m_console->SetBackgroundColor(0);
	m_console->SetTextColor(ConRed | ConGreen);
	m_position.X = 30;
	m_position.Y = 7;
	m_console->SetPosition(m_position); // Set coordinates
	sprintf(m_buffer, "North-South:%d", m_playerWinAndLostRecord[0]);
	m_console->OutputString(m_buffer);

	m_position.X = 30;
	m_position.Y = 8;
	m_console->SetPosition(m_position); // Set coordinates
	memset(m_buffer, 0, 100);
	sprintf(m_buffer, "West-East:%d", m_playerWinAndLostRecord[1]);
	m_console->OutputString(m_buffer);

	m_position.X = 30;
	m_position.Y = 9;
	m_console->SetPosition(m_position); // Set coordinates
	m_console->OutputString("Contract:");
	m_console->OutputString(&m_previousTonAndSuit.ton, 1); // Output the number of tricks

	m_position.X = 30;
	m_position.Y = 16;
	m_console->SetPosition(m_position); // Set coordinates
	if (m_playerWinAndLostRecord[0] > m_previousTonAndSuit.ton - 48 + 6)
		m_console->OutputString("North-South win!");
	else if (m_playerWinAndLostRecord[1] > m_previousTonAndSuit.ton - 48 + 6)
		m_console->OutputString("West-East win!"); // Output suit

	m_position.X = 57;
	m_position.Y = 23;
	m_console->SetPosition(m_position);								// Set coordinates
	m_console->SetTextColor(ConRed);								// Set text colour
	m_console->OutputString("-->Press ENTER to exit the game!<--"); // Output text
}

void Deck::ShowExit()
{
	COORD m_postion = {26, 9};
	m_console->SetBackgroundColor(0);
	m_console->SetTextColor(ConRed | ConBlue | ConGreen);
	m_console->Clear();
	m_console->SetPosition(m_postion);
	m_console->OutputString("--> Restart <- \n");
	m_postion.Y++;
	m_console->SetPosition(m_postion);
	m_console->OutputString("-_-\n");
	m_postion.Y++;
	m_console->SetPosition(m_postion);
	m_console->OutputString("^O^\n");
	m_postion.Y++;
	m_console->SetPosition(m_postion);
	m_console->OutputString("Bye!!!\n");

	delete g_pGameMusicOperate;
}

void Deck::Show()
{
	m_console->SetBackgroundColor(0);
	m_console->SetTextColor(ConRed | ConGreen | ConBlue);
	m_console->Clear();
	switch (m_gameStatus)
	{
	case GameSplashScreen:
		ShowSplash();
		break;
	case GameMainMenu:
		ShowMenu();
		break;
	case GameCallCard:
		ShowCall();
		break;
	case GameRunning:
		ShowGame();
		break;
	case GameOver:
		ShowOver();
		break;
	case GameExit:
		ShowExit();
		break;
	default:
		break;
	}
}
