#include "HUD.h"

#include "raylib.h"
#include "ui/FantasyText.h"

HUD::HUD()
	: healthBarWidth(80.0f),
	  healthBarHeight(8.0f)
{
}

void HUD::Draw(int coins, int highScore, int waveNumber, const Player &player) const
{
	DrawScores(coins, highScore, waveNumber);
	DrawPlayerHealthBar(player);
}

void HUD::DrawScores(int coins, int highScore, int waveNumber) const
{
	DrawFantasyText(TextFormat("Coins: %d", coins), 16, 14, 24, GOLD);
	DrawFantasyText(TextFormat("High Score: %d", highScore), 16, 42, 22, WHITE);
	DrawFantasyText(TextFormat("Wave: %d", waveNumber), 16, 68, 22, RED);
}

void HUD::DrawPlayerHealthBar(const Player &player) const
{
	Vector2 playerPosition = player.GetPosition();
	float playerSize = player.GetSize();
	float healthBarX = playerPosition.x - healthBarWidth / 2.0f;
	float healthBarY = playerPosition.y - playerSize / 2.0f - 18.0f;
	float healthPercent = (float)player.GetHealth() / (float)player.GetMaxHealth();

	DrawRectangleRec({healthBarX, healthBarY, healthBarWidth, healthBarHeight}, DARKGRAY);
	DrawRectangleRec({healthBarX, healthBarY, healthBarWidth * healthPercent, healthBarHeight}, LIME);
	DrawRectangleLinesEx({healthBarX, healthBarY, healthBarWidth, healthBarHeight}, 1.0f, BLACK);
}
