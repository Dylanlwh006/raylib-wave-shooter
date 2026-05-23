#pragma once

#include "raylib.h"

struct SaveData
{
	int coins;
	Vector2 playerPosition;
	int playerHealth;
	float playerShootCooldown;
	int waveNextLoadingIndex;
	bool waveLoading;
	bool waveSpawnWaveRequested;
	bool waveCurrentSpawnHasLoadingScreen;
	bool waveCompleted;
	int enemiesRemainingInWave;
	int activeEnemyCount;
	bool activeEnemyIsBoss[2];
	bool activeEnemyIsFast[2];
	bool activeEnemyIsTank[2];
	Vector2 enemyPosition[2];
	int enemyHealth[2];
};

class SaveManager
{
public:
	static bool SaveGame(const char *filePath, const SaveData &data);
	static bool LoadGame(const char *filePath, SaveData &data);
	static int LoadHighScore(const char *filePath);
	static bool SaveHighScore(const char *filePath, int highScore);
};
