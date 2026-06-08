#include "Game.h"

#include "core/Constants.h"
#include "raymath.h"
#include "systems/Collision.h"

#include <cmath>

// Advances active gameplay and resolves projectile and player collisions.
void Game::Update(float deltaTime, Vector2 mousePosition)
{
	if (currentState == GameState::Paused)
	{
		return;
	}

	if (!player->IsAlive())
	{
		currentState = GameState::GameOver;
		return;
	}

	if (waveManager.IsLoading())
	{
		currentState = GameState::Loading;
		if (waveManager.ShouldSpawnWave())
		{
			SpawnWave();
			waveManager.ConsumeSpawnRequest();
			currentState = GameState::Playing;
		}
		return;
	}

	if (waveManager.IsCompleted())
	{
		currentState = GameState::Victory;
		return;
	}

	if (waveManager.ShouldSpawnWave())
	{
		SpawnWave();
		waveManager.ConsumeSpawnRequest();
		currentState = GameState::Playing;
	}

	player->UpdateTimers(deltaTime);
	player->AimAt(mousePosition);

	if (player->IsAlive() && !shop->IsOpen())
	{
		currentState = GameState::Playing;
		player->HandleMovement(deltaTime, screenWidth, screenHeight);

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && player->CanShoot())
		{
			Vector2 aimDirection = GetDirection(player->GetPosition(), mousePosition);
			float aimAngle = atan2f(aimDirection.y, aimDirection.x);
			Vector2 bulletPosition = Vector2Add(player->GetPosition(), Vector2Scale(aimDirection, player->GetSize() * 0.4f));

			bullets.push_back(Bullet(
				&bulletTexture,
				bulletPosition,
				Vector2Scale(aimDirection, bulletSpeed),
				bulletSize,
				aimAngle * RAD2DEG + bulletRotationOffset,
				bulletDamage));

			player->ResetShootTimer();
		}

		for (std::unique_ptr<Enemy> &enemy : enemies)
		{
			enemy->Update(player->GetPosition(), deltaTime);
		}
	}

	if (!shop->IsOpen())
	{
		UpdateBullets(deltaTime);
		CheckBulletEnemyCollisions();
		CheckPlayerEnemyCollisions();
	}
}

void Game::UpdateBullets(float deltaTime)
{
	for (int i = (int)bullets.size() - 1; i >= 0; i--)
	{
		bullets[i].Update(deltaTime);

		if (bullets[i].IsOffScreen(screenWidth, screenHeight))
		{
			bullets.erase(bullets.begin() + i);
		}
	}
}

void Game::CheckBulletEnemyCollisions()
{
	// Iterate backward so bullets and enemies can be removed safely after hits.
	for (int i = (int)bullets.size() - 1; i >= 0; i--)
	{
		bool bulletHitEnemy = false;

		for (int enemyIndex = (int)enemies.size() - 1; enemyIndex >= 0; enemyIndex--)
		{
			Enemy &enemy = *enemies[enemyIndex];

			if (CheckCollisionRecs(bullets[i].GetHitbox(), enemy.GetHitbox()))
			{
				bulletHitEnemy = true;
				enemy.TakeDamage(bullets[i].GetDamage());

				if (enemy.IsDead())
				{
					coins += coinsPerKill;
					UpdateHighScore();
					bool killedBoss = enemy.IsBoss();
					enemies.erase(enemies.begin() + enemyIndex);
					if (!killedBoss)
					{
						SpawnEnemiesToActiveLimit();
					}
				}

				break;
			}
		}

		if (bulletHitEnemy)
		{
			bullets.erase(bullets.begin() + i);
		}
	}

	if (player->IsAlive() && enemies.empty() && enemiesRemainingInWave <= 0)
	{
		bullets.clear();
		waveManager.StartNextWave();
	}
}

void Game::CheckPlayerEnemyCollisions()
{
	if (!player->IsAlive())
	{
		return;
	}

	for (std::unique_ptr<Enemy> &enemy : enemies)
	{
		if (Collision::CheckPlayerEnemyCollision(*player, *enemy))
		{
			player->TakeDamage(enemy->GetAttackDamage());
			enemy->Reset(GetRandomEnemyPosition());
			bullets.clear();
		}
	}
}
