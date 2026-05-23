#include "SaveManager.h"

#include <fstream>

bool SaveManager::SaveGame(const char *filePath, const SaveData &data)
{
	std::ofstream saveFile(filePath);
	if (!saveFile.is_open())
	{
		return false;
	}

	saveFile << data.coins << '\n';
	saveFile << data.playerPosition.x << ' ' << data.playerPosition.y << '\n';
	saveFile << data.playerHealth << '\n';
	saveFile << data.playerShootCooldown << '\n';
	saveFile << data.waveNextLoadingIndex << '\n';
	saveFile << data.waveLoading << ' ' << data.waveSpawnWaveRequested << ' ' << data.waveCurrentSpawnHasLoadingScreen << ' ' << data.waveCompleted << '\n';
	saveFile << data.enemiesRemainingInWave << '\n';
	saveFile << data.activeEnemyCount << '\n';
	for (int i = 0; i < data.activeEnemyCount && i < 2; i++)
	{
		saveFile << data.activeEnemyIsBoss[i] << '\n';
		saveFile << data.activeEnemyIsFast[i] << '\n';
		saveFile << data.activeEnemyIsTank[i] << '\n';
		saveFile << data.enemyPosition[i].x << ' ' << data.enemyPosition[i].y << '\n';
		saveFile << data.enemyHealth[i] << '\n';
	}

	return true;
}

bool SaveManager::LoadGame(const char *filePath, SaveData &data)
{
	std::ifstream saveFile(filePath);
	if (!saveFile.is_open())
	{
		return false;
	}

	saveFile >> data.coins;
	saveFile >> data.playerPosition.x >> data.playerPosition.y;
	saveFile >> data.playerHealth;
	saveFile >> data.playerShootCooldown;
	saveFile >> data.waveNextLoadingIndex;
	saveFile >> data.waveLoading >> data.waveSpawnWaveRequested >> data.waveCurrentSpawnHasLoadingScreen >> data.waveCompleted;
	saveFile >> data.enemiesRemainingInWave;
	saveFile >> data.activeEnemyCount;
	if (data.activeEnemyCount < 0)
	{
		data.activeEnemyCount = 0;
	}
	if (data.activeEnemyCount > 2)
	{
		data.activeEnemyCount = 2;
	}

	for (int i = 0; i < 2; i++)
	{
		data.activeEnemyIsBoss[i] = false;
		data.activeEnemyIsFast[i] = false;
		data.activeEnemyIsTank[i] = false;
		data.enemyPosition[i] = Vector2{0.0f, 0.0f};
		data.enemyHealth[i] = 0;
	}

	for (int i = 0; i < data.activeEnemyCount; i++)
	{
		saveFile >> data.activeEnemyIsBoss[i];
		saveFile >> data.activeEnemyIsFast[i];
		saveFile >> data.activeEnemyIsTank[i];
		saveFile >> data.enemyPosition[i].x >> data.enemyPosition[i].y;
		saveFile >> data.enemyHealth[i];
	}

	return !saveFile.fail();
}

int SaveManager::LoadHighScore(const char *filePath)
{
	std::ifstream highScoreFile(filePath);
	if (!highScoreFile.is_open())
	{
		return 0;
	}

	int highScore = 0;
	highScoreFile >> highScore;
	if (highScoreFile.fail() || highScore < 0)
	{
		return 0;
	}

	return highScore;
}

bool SaveManager::SaveHighScore(const char *filePath, int highScore)
{
	std::ofstream highScoreFile(filePath);
	if (!highScoreFile.is_open())
	{
		return false;
	}

	highScoreFile << highScore << '\n';
	return true;
}
