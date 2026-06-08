#include "Game.h"

#include "core/Constants.h"

// Handles menus and persistence actions that change the current game state.
void Game::TogglePause()
{
	if (!startScreen->IsActive() && !waveManager.IsLoading() && !waveManager.IsCompleted() && player->IsAlive())
	{
		if (currentState == GameState::Paused)
		{
			currentState = previousState;
		}
		else
		{
			previousState = currentState;
			currentState = GameState::Paused;
		}
	}
}

void Game::SaveGame()
{
	// Collect the current session into a plain data structure for file saving.
	SaveData data = {};
	data.coins = coins;
	data.playerPosition = player->GetPosition();
	data.playerHealth = player->GetHealth();
	data.playerShootCooldown = player->GetShootCooldown();
	data.waveNextLoadingIndex = waveManager.GetNextLoadingIndex();
	data.waveLoading = waveManager.IsLoading();
	data.waveSpawnWaveRequested = waveManager.ShouldSpawnWave();
	data.waveCurrentSpawnHasLoadingScreen = waveManager.CurrentSpawnHasLoadingScreen();
	data.waveCompleted = waveManager.IsCompleted();
	data.enemiesRemainingInWave = enemiesRemainingInWave;
	data.activeEnemyCount = (int)enemies.size();
	if (data.activeEnemyCount > Constants::MAX_ACTIVE_ENEMIES)
	{
		data.activeEnemyCount = Constants::MAX_ACTIVE_ENEMIES;
	}

	for (int i = 0; i < Constants::MAX_ACTIVE_ENEMIES; i++)
	{
		data.activeEnemyIsBoss[i] = false;
		data.activeEnemyIsFast[i] = false;
		data.activeEnemyIsTank[i] = false;
		data.enemyPosition[i] = Vector2{0.0f, 0.0f};
		data.enemyHealth[i] = 0;
	}

	for (int i = 0; i < data.activeEnemyCount; i++)
	{
		data.activeEnemyIsBoss[i] = enemies[i]->IsBoss();
		data.activeEnemyIsFast[i] = enemies[i]->IsFast();
		data.activeEnemyIsTank[i] = enemies[i]->IsTank();
		data.enemyPosition[i] = enemies[i]->GetPosition();
		data.enemyHealth[i] = enemies[i]->GetHealth();
	}

	SaveManager::SaveGame(saveFilePath, data);
}

void Game::LoadGame()
{
	SaveData data = {};
	if (!SaveManager::LoadGame(saveFilePath, data))
	{
		return;
	}

	coins = data.coins;
	player->SetPosition(data.playerPosition);
	player->SetHealth(data.playerHealth);
	player->SetShootCooldown(data.playerShootCooldown);
	player->ResetShootTimer();
	waveManager.SetState(
		data.waveNextLoadingIndex,
		data.waveLoading,
		data.waveSpawnWaveRequested,
		data.waveCurrentSpawnHasLoadingScreen,
		data.waveCompleted);
	enemiesRemainingInWave = data.enemiesRemainingInWave;
	bullets.clear();
	enemies.clear();

	for (int i = 0; i < data.activeEnemyCount; i++)
	{
		if (data.activeEnemyIsBoss[i])
		{
			enemies.push_back(std::make_unique<Boss>(
				&bossTexture,
				data.enemyPosition[i],
				bossSize,
				bossSpeed,
				basicEnemyHitboxScale,
				bossMaxHealth,
				bossAttackDamage,
				basicEnemyRotationOffset));
		}
		else if (data.activeEnemyIsFast[i])
		{
			enemies.push_back(std::make_unique<FastEnemy>(
				&fastEnemyTexture,
				data.enemyPosition[i],
				basicEnemySize,
				fastEnemySpeed,
				basicEnemyHitboxScale,
				fastEnemyMaxHealth,
				basicEnemyAttackDamage,
				basicEnemyRotationOffset));
		}
		else if (data.activeEnemyIsTank[i])
		{
			enemies.push_back(std::make_unique<TankEnemy>(
				&tankEnemyTexture,
				data.enemyPosition[i],
				basicEnemySize,
				tankEnemySpeed,
				basicEnemyHitboxScale,
				tankEnemyMaxHealth,
				basicEnemyAttackDamage,
				basicEnemyRotationOffset));
		}
		else
		{
			enemies.push_back(std::make_unique<BasicEnemy>(
				&basicEnemyTexture,
				data.enemyPosition[i],
				basicEnemySize,
				basicEnemySpeed,
				basicEnemyHitboxScale,
				basicEnemyMaxHealth,
				basicEnemyAttackDamage,
				basicEnemyRotationOffset));
		}

		enemies.back()->SetHealth(data.enemyHealth[i]);
	}

	currentState = waveManager.IsCompleted() ? GameState::Victory : GameState::Playing;
	UpdateHighScore();
}

void Game::UpdateHighScore()
{
	if (coins > highScore)
	{
		highScore = coins;
		SaveManager::SaveHighScore(highScoreFilePath, highScore);
	}
}

void Game::HandleInput(Vector2 mousePosition)
{
	if (IsKeyPressed(KEY_P))
	{
		TogglePause();
	}

	if (currentState == GameState::Paused)
	{
		if (resumeButton.IsClicked(mousePosition))
		{
			currentState = previousState;
		}
		else if (saveButton.IsClicked(mousePosition))
		{
			SaveGame();
		}
		else if (loadButton.IsClicked(mousePosition))
		{
			LoadGame();
		}
		return;
	}

	if (!player->IsAlive())
	{
		currentState = GameState::GameOver;
		if (restartButton.IsClicked(mousePosition))
		{
			ResetGame();
		}
		return;
	}

	if (waveManager.IsLoading())
	{
		currentState = GameState::Loading;
		waveManager.HandleInput();
		return;
	}

	shop->HandleInput(mousePosition, coins, *player);
}
