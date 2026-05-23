#pragma once

#include "raylib.h"

#include <vector>

class WaveManager
{
private:
	std::vector<Texture2D *> levelTextures;
	int nextLoadingIndex;
	int enemiesPerWave;
	bool loading;
	bool spawnWaveRequested;
	bool currentSpawnHasLoadingScreen;
	bool completed;
	int screenWidth;
	int screenHeight;

public:
	WaveManager();
	WaveManager(const std::vector<Texture2D *> &textures, int enemiesEachWave, int windowWidth, int windowHeight);

	void StartFirstWave();
	void StartNextWave();
	void SetState(int loadingIndex, bool isLoading, bool shouldSpawnWave, bool spawnHasLoadingScreen, bool isCompleted);
	void HandleInput();
	void DrawLoadingScreen() const;

	bool IsLoading() const;
	bool IsCompleted() const;
	bool ShouldSpawnWave() const;
	void ConsumeSpawnRequest();
	int GetNextLoadingIndex() const;
	bool CurrentSpawnHasLoadingScreen() const;
	int GetEnemiesPerWave() const;
	int GetCurrentWaveNumber() const;
	int GetEnemyCountForCurrentWave() const;
};
