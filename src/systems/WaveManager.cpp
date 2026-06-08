#include "WaveManager.h"

#include "core/Constants.h"

WaveManager::WaveManager()
	: nextLoadingIndex(0),
	  enemiesPerWave(5),
	  loading(false),
	  spawnWaveRequested(false),
	  currentSpawnHasLoadingScreen(false),
	  completed(false),
	  screenWidth(800),
	  screenHeight(600)
{
}

WaveManager::WaveManager(const std::vector<Texture2D *> &textures, int enemiesEachWave, int windowWidth, int windowHeight)
	: levelTextures(textures),
	  nextLoadingIndex(0),
	  enemiesPerWave(enemiesEachWave),
	  loading(false),
	  spawnWaveRequested(false),
	  currentSpawnHasLoadingScreen(false),
	  completed(false),
	  screenWidth(windowWidth),
	  screenHeight(windowHeight)
{
}

void WaveManager::StartFirstWave()
{
	// The first wave begins immediately without showing a loading screen.
	nextLoadingIndex = 0;
	loading = false;
	spawnWaveRequested = true;
	currentSpawnHasLoadingScreen = false;
	completed = false;
}

void WaveManager::StartNextWave()
{
	// Later waves pause on a loading screen before requesting enemy spawning.
	if (nextLoadingIndex >= (int)levelTextures.size())
	{
		completed = true;
		loading = false;
		spawnWaveRequested = false;
		return;
	}

	loading = true;
	spawnWaveRequested = false;
	currentSpawnHasLoadingScreen = true;
}

void WaveManager::SetState(int loadingIndex, bool isLoading, bool shouldSpawnWave, bool spawnHasLoadingScreen, bool isCompleted)
{
	nextLoadingIndex = loadingIndex;
	if (nextLoadingIndex < 0)
	{
		nextLoadingIndex = 0;
	}
	if (!levelTextures.empty() && nextLoadingIndex > (int)levelTextures.size())
	{
		nextLoadingIndex = (int)levelTextures.size();
	}

	loading = isLoading;
	spawnWaveRequested = shouldSpawnWave;
	currentSpawnHasLoadingScreen = spawnHasLoadingScreen;
	completed = isCompleted;
}

void WaveManager::HandleInput()
{
	if (!loading)
	{
		return;
	}

	if (GetKeyPressed() != 0 || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		// After wave 9, show the level_9 screen first, then the boss screen.
		if (nextLoadingIndex == Constants::BOSS_LEVEL - 2 && (int)levelTextures.size() >= Constants::BOSS_LEVEL)
		{
			nextLoadingIndex++;
			return;
		}

		loading = false;
		spawnWaveRequested = true;
	}
}

void WaveManager::DrawLoadingScreen() const
{
	if (levelTextures.empty())
	{
		ClearBackground(BLACK);
		return;
	}

	int loadingIndex = nextLoadingIndex;
	if (loadingIndex >= (int)levelTextures.size())
	{
		loadingIndex = (int)levelTextures.size() - 1;
	}

	Texture2D *levelTexture = levelTextures[loadingIndex];
	DrawTexturePro(
		*levelTexture,
		{0.0f, 0.0f, (float)levelTexture->width, (float)levelTexture->height},
		{0.0f, 0.0f, (float)screenWidth, (float)screenHeight},
		{0.0f, 0.0f},
		0.0f,
		WHITE);
}

bool WaveManager::IsLoading() const
{
	return loading;
}

bool WaveManager::IsCompleted() const
{
	return completed;
}

bool WaveManager::ShouldSpawnWave() const
{
	return spawnWaveRequested;
}

void WaveManager::ConsumeSpawnRequest()
{
	spawnWaveRequested = false;
	if (currentSpawnHasLoadingScreen && nextLoadingIndex < (int)levelTextures.size())
	{
		nextLoadingIndex++;
	}
	currentSpawnHasLoadingScreen = false;
}

int WaveManager::GetNextLoadingIndex() const
{
	return nextLoadingIndex;
}

bool WaveManager::CurrentSpawnHasLoadingScreen() const
{
	return currentSpawnHasLoadingScreen;
}

int WaveManager::GetEnemiesPerWave() const
{
	return enemiesPerWave;
}

int WaveManager::GetCurrentWaveNumber() const
{
	if (currentSpawnHasLoadingScreen)
	{
		int nextWaveNumber = nextLoadingIndex + 2;
		if (nextWaveNumber > Constants::BOSS_LEVEL)
		{
			return Constants::BOSS_LEVEL;
		}

		return nextWaveNumber;
	}

	int currentWaveNumber = nextLoadingIndex + 1;
	if (currentWaveNumber > Constants::BOSS_LEVEL)
	{
		return Constants::BOSS_LEVEL;
	}

	return currentWaveNumber;
}

int WaveManager::GetEnemyCountForCurrentWave() const
{
	// Normal waves grow by one enemy each level; the boss wave uses no normal enemies.
	if (GetCurrentWaveNumber() >= Constants::BOSS_LEVEL)
	{
		return 0;
	}

	return enemiesPerWave + GetCurrentWaveNumber() - 1;
}
