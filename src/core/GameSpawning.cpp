#include "Game.h"

#include "core/Constants.h"

// Creates wave enemies while enforcing progression and active-enemy limits.
void Game::SpawnWave()
{
	enemies.clear();
	if (IsBossWave())
	{
		enemiesRemainingInWave = 0;
		SpawnBoss();
		return;
	}

	enemiesRemainingInWave = waveManager.GetEnemyCountForCurrentWave();
	SpawnEnemiesToActiveLimit();
}

void Game::SpawnEnemy()
{
	if (enemiesRemainingInWave <= 0)
	{
		return;
	}

	// Enemy type changes by wave so difficulty increases gradually.
	int currentLevel = waveManager.GetCurrentWaveNumber();
	int enemyType = 0;
	if (currentLevel >= 7)
	{
		int roll = GetRandomValue(1, 100);
		if (roll <= 20)
		{
			enemyType = 0;
		}
		else if (roll <= 60)
		{
			enemyType = 2;
		}
		else
		{
			enemyType = 1;
		}
	}
	else if (currentLevel >= 4)
	{
		enemyType = GetRandomValue(1, 100) <= 25 ? 0 : 2;
	}

	if (enemyType == 0)
	{
		enemies.push_back(std::make_unique<BasicEnemy>(
			&basicEnemyTexture,
			GetRandomEnemyPosition(),
			basicEnemySize,
			basicEnemySpeed,
			basicEnemyHitboxScale,
			basicEnemyMaxHealth,
			basicEnemyAttackDamage,
			basicEnemyRotationOffset));
	}
	else if (enemyType == 1)
	{
		enemies.push_back(std::make_unique<FastEnemy>(
			&fastEnemyTexture,
			GetRandomEnemyPosition(),
			basicEnemySize,
			fastEnemySpeed,
			basicEnemyHitboxScale,
			fastEnemyMaxHealth,
			basicEnemyAttackDamage,
			basicEnemyRotationOffset));
	}
	else
	{
		enemies.push_back(std::make_unique<TankEnemy>(
			&tankEnemyTexture,
			GetRandomEnemyPosition(),
			basicEnemySize,
			tankEnemySpeed,
			basicEnemyHitboxScale,
			tankEnemyMaxHealth,
			basicEnemyAttackDamage,
			basicEnemyRotationOffset));
	}

	enemiesRemainingInWave--;
}

void Game::SpawnEnemiesToActiveLimit()
{
	int activeEnemyTarget = GetRandomValue(1, Constants::MAX_ACTIVE_ENEMIES);
	while (enemiesRemainingInWave > 0 && (int)enemies.size() < activeEnemyTarget)
	{
		SpawnEnemy();
	}
}

void Game::SpawnBoss()
{
	enemies.push_back(std::make_unique<Boss>(
		&bossTexture,
		GetRandomEnemyPosition(),
		bossSize,
		bossSpeed,
		basicEnemyHitboxScale,
		bossMaxHealth,
		bossAttackDamage,
		basicEnemyRotationOffset));
}

bool Game::IsBossWave() const
{
	return waveManager.GetCurrentWaveNumber() == Constants::BOSS_LEVEL;
}
