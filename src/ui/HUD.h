#pragma once

#include "entities/Player.h"

class HUD
{
private:
	float healthBarWidth;
	float healthBarHeight;

public:
	HUD();

	void Draw(int coins, int highScore, int waveNumber, const Player &player) const;
	void DrawScores(int coins, int highScore, int waveNumber) const;
	void DrawPlayerHealthBar(const Player &player) const;
};
