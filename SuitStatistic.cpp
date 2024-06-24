#include "SuitStatistic.h"

SuitStatistic::SuitStatistic()
{
	// Initialise to -1
	m_suitSpadeNum = -1;
	m_suitHeartNum = -1;
	m_suitClubNum = -1;
	m_suitDiamondNum = -1;
	m_pointsAndShapeFigure = -1;
	m_player = -1;
}
SuitStatistic::~SuitStatistic() {}

int SuitStatistic::GetSuitSpadeNum() // Get the number of spades
{
	return m_suitSpadeNum;
}
void SuitStatistic::SetSuitSpadeNum(int suitSpade) // Set the number of spades
{
	m_suitSpadeNum = suitSpade;
}

int SuitStatistic::GetSuitHeartNum() // Get the number of hearts
{
	return m_suitHeartNum;
}
void SuitStatistic::SetSuitHeartNum(int heartNum) // Set the number of hearts
{
	m_suitHeartNum = heartNum;
}

int SuitStatistic::GetSuitClubNum() // Get the number of clubs
{
	return m_suitClubNum;
}
void SuitStatistic::SetSuitClubNum(int clubNum) // Set the number of clubs
{
	m_suitClubNum = clubNum;
}

int SuitStatistic::GetSuitDiamondNum() // Get the number of diamonds
{
	return m_suitDiamondNum;
}
void SuitStatistic::SetSuitDiamondNum(int diamondNum) // Set the number of diamonds
{
	m_suitDiamondNum = diamondNum;
}

int SuitStatistic::GetPointsAndShapeFigure() // Get the player's card points
{
	return m_pointsAndShapeFigure;
}
void SuitStatistic::SetPointsAndShapeFigure(int pointsAndShapeFigure) // Set the player's card points
{
	m_pointsAndShapeFigure = pointsAndShapeFigure;
}

int SuitStatistic::GetPlayer() // Get the current player
{
	return m_player;
}
void SuitStatistic::SetPlayer(int player) // Set the current player
{
	m_player = player;
}
