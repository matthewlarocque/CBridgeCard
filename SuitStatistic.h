#pragma once 

#ifndef _SUITSTATISTIC_H_
#define _SUITSTATISTIC_H_ 1

class SuitStatistic
{
public:
	SuitStatistic();
	virtual ~SuitStatistic();

	int GetSuitSpadeNum(); // Get the number of spades
	void SetSuitSpadeNum(int suitSpade); // Set the number of spades

	int GetSuitHeartNum(); // Get the number of hearts
	void SetSuitHeartNum(int heartNum); // Set the number of hearts

	int GetSuitClubNum(); // Get the number of clubs
	void SetSuitClubNum(int clubNum); // Set the number of clubs

	int GetSuitDiamondNum(); // Get the number of diamonds
	void SetSuitDiamondNum(int diamondNum); // Set the number of diamonds

	int GetPointsAndShapeFigure(); // Get the player's card points, summing cards above 10
	void SetPointsAndShapeFigure(int pointsAndShapeFigure); // Set the player's card points

	int GetPlayer(); // Get the current player
	void SetPlayer(int player); // Set the current player

private:
	int m_suitSpadeNum; // Number of spades
	int m_suitHeartNum; // Number of hearts
	int m_suitClubNum; // Number of clubs
	int m_suitDiamondNum; // Number of diamonds
	int m_pointsAndShapeFigure; // Player's card points

	int m_player; // Current player
};

#endif
