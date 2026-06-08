#include "Game.h"

#include "ui/FantasyText.h"

// Draws the active screen and overlays without changing gameplay state.
void Game::Draw(Vector2 mousePosition) const
{
	BeginDrawing();
	ClearBackground(BLACK);

	if (startScreen->IsActive())
	{
		startScreen->Draw();
		EndDrawing();
		return;
	}

	if (waveManager.IsLoading())
	{
		waveManager.DrawLoadingScreen();
		hud.DrawScores(coins, highScore, waveManager.GetCurrentWaveNumber());
		shop->Draw(mousePosition, coins);
		EndDrawing();
		return;
	}

	if (waveManager.IsCompleted())
	{
		DrawVictoryScreen();
		EndDrawing();
		return;
	}

	if (currentState == GameState::GameOver || !player->IsAlive())
	{
		DrawGameOverScreen(mousePosition);
		EndDrawing();
		return;
	}

	DrawTexturePro(
		backgroundTexture,
		{0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height},
		{0.0f, 0.0f, (float)screenWidth, (float)screenHeight},
		{0.0f, 0.0f},
		0.0f,
		WHITE);

	hud.Draw(coins, highScore, waveManager.GetCurrentWaveNumber(), *player);

	for (const Bullet &bullet : bullets)
	{
		bullet.Draw();
	}

	for (const std::unique_ptr<Enemy> &enemy : enemies)
	{
		enemy->Draw();
	}

	player->Draw();
	shop->Draw(mousePosition, coins);

	if (currentState == GameState::Paused)
	{
		DrawPauseScreen(mousePosition);
	}

	EndDrawing();
}

void Game::DrawGameOverScreen(Vector2 mousePosition) const
{
	DrawTexturePro(
		gameOverTexture,
		{0.0f, 0.0f, (float)gameOverTexture.width, (float)gameOverTexture.height},
		{0.0f, 0.0f, (float)screenWidth, (float)screenHeight},
		{0.0f, 0.0f},
		0.0f,
		WHITE);
	DrawEndScreenScores();
	Rectangle restartBounds = restartButton.GetBounds();
	Color restartTint = restartButton.IsHovered(mousePosition) ? WHITE : Fade(WHITE, 0.86f);
	DrawTexturePro(
		restartTexture,
		{0.0f, 0.0f, (float)restartTexture.width, (float)restartTexture.height},
		restartBounds,
		{0.0f, 0.0f},
		0.0f,
		restartTint);
}

void Game::DrawPauseScreen(Vector2 mousePosition) const
{
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.62f));

	const char *pauseText = "PAUSED";
	int pauseFontSize = 44;
	int pauseTextWidth = MeasureFantasyText(pauseText, pauseFontSize);
	DrawFantasyText(pauseText, screenWidth / 2 - pauseTextWidth / 2, screenHeight / 2 - 92, pauseFontSize, WHITE);

	resumeButton.Draw(mousePosition);
	saveButton.Draw(mousePosition);
	loadButton.Draw(mousePosition);
}

void Game::DrawVictoryScreen() const
{
	DrawTexturePro(
		victoryTexture,
		{0.0f, 0.0f, (float)victoryTexture.width, (float)victoryTexture.height},
		{0.0f, 0.0f, (float)screenWidth, (float)screenHeight},
		{0.0f, 0.0f},
		0.0f,
		WHITE);
	DrawEndScreenScores();
}

void Game::DrawEndScreenScores() const
{
	const char *scoreText = TextFormat("Score: %d", coins);
	int scoreFontSize = 26;
	int scoreTextWidth = MeasureFantasyText(scoreText, scoreFontSize);
	DrawFantasyText(scoreText, screenWidth / 2 - scoreTextWidth / 2, screenHeight / 2 + 100, scoreFontSize, GOLD);

	const char *highScoreText = TextFormat("High Score: %d", highScore);
	int highScoreFontSize = 24;
	int highScoreTextWidth = MeasureFantasyText(highScoreText, highScoreFontSize);
	DrawFantasyText(highScoreText, screenWidth / 2 - highScoreTextWidth / 2, screenHeight / 2 + 132, highScoreFontSize, WHITE);
}
