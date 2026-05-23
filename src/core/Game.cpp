#include "Game.h"

#include "core/Constants.h"
#include "raymath.h"
#include "resource_dir.h"
#include "systems/Collision.h"
#include "ui/FantasyText.h"

#include <cmath>

Game::Game()
	: screenWidth(Constants::SCREEN_WIDTH),
	  screenHeight(Constants::SCREEN_HEIGHT),
	  backgroundTexture({}),
	  playerTexture({}),
	  bulletTexture({}),
	  basicEnemyTexture({}),
	  fastEnemyTexture({}),
	  tankEnemyTexture({}),
	  bossTexture({}),
	  shopTexture({}),
	  medkitTexture({}),
	  bulletMagTexture({}),
	  startTexture({}),
	  victoryTexture({}),
	  gameOverTexture({}),
	  restartTexture({}),
	  levelTextures{},
	  soundtrack({}),
	  playerStartPosition({screenWidth / 2.0f, screenHeight / 2.0f}),
	  restartButton({screenWidth / 2.0f - 80.0f, screenHeight / 2.0f + 44.0f, 160.0f, 44.0f}, "Restart", 22, BLUE, SKYBLUE, WHITE, WHITE),
	  resumeButton({screenWidth / 2.0f - 90.0f, screenHeight / 2.0f - 20.0f, 180.0f, 42.0f}, "Resume", 22, BLUE, SKYBLUE, WHITE, WHITE),
	  saveButton({screenWidth / 2.0f - 90.0f, screenHeight / 2.0f + 36.0f, 180.0f, 42.0f}, "Save", 22, DARKGREEN, GREEN, WHITE, WHITE),
	  loadButton({screenWidth / 2.0f - 90.0f, screenHeight / 2.0f + 92.0f, 180.0f, 42.0f}, "Load", 22, DARKPURPLE, PURPLE, WHITE, WHITE),
	  currentState(GameState::StartScreen),
	  previousState(GameState::Playing),
	  playerSize(Constants::PLAYER_SIZE),
	  bulletSpeed(Constants::BULLET_SPEED),
	  bulletSize(Constants::BULLET_SIZE),
	  bulletRotationOffset(Constants::BULLET_ROTATION_OFFSET),
	  basicEnemySize(playerSize * Constants::ENEMY_SIZE_MULTIPLIER),
	  basicEnemySpeed(Constants::BASIC_ENEMY_SPEED),
	  basicEnemyHitboxScale(Constants::ENEMY_HITBOX_SCALE),
	  basicEnemyRotationOffset(Constants::ENEMY_ROTATION_OFFSET),
	  fastEnemySpeed(basicEnemySpeed * Constants::FAST_ENEMY_SPEED_MULTIPLIER),
	  tankEnemySpeed(basicEnemySpeed * Constants::TANK_ENEMY_SPEED_MULTIPLIER),
	  bossSize(basicEnemySize * Constants::BOSS_SIZE_MULTIPLIER),
	  bossSpeed(Constants::BOSS_SPEED),
	  bulletDamage(Constants::BULLET_DAMAGE),
	  basicEnemyMaxHealth(Constants::BASIC_ENEMY_HEALTH),
	  fastEnemyMaxHealth(Constants::FAST_ENEMY_HEALTH),
	  tankEnemyMaxHealth(Constants::TANK_ENEMY_HEALTH),
	  basicEnemyAttackDamage(Constants::BASIC_ENEMY_DAMAGE),
	  bossMaxHealth(Constants::BOSS_HEALTH),
	  bossAttackDamage(Constants::BOSS_DAMAGE),
	  basicEnemyCount(Constants::ENEMIES_PER_WAVE),
	  enemiesRemainingInWave(0),
	  coinsPerKill(Constants::COINS_PER_KILL),
	  coins(0),
	  saveFilePath("../data/save.txt"),
	  highScoreFilePath("../data/highscore.txt"),
	  highScore(0),
	  soundtrackLoaded(false)
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(screenWidth, screenHeight, "Freedom");
	InitAudioDevice();
	SearchAndSetResourceDir("resources");

	highScore = SaveManager::LoadHighScore(highScoreFilePath);
	LoadTextures();
	CreateGameObjects();
}

Game::~Game()
{
	enemies.clear();
	bullets.clear();
	startScreen.reset();
	shop.reset();
	player.reset();
	if (soundtrackLoaded)
	{
		UnloadMusicStream(soundtrack);
	}
	UnloadTextures();
	CloseAudioDevice();
	CloseWindow();
}

void Game::run()
{
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		Vector2 mousePosition = GetMousePosition();
		if (soundtrackLoaded)
		{
			UpdateMusicStream(soundtrack);
		}

	if (startScreen->IsActive())
	{
		startScreen->HandleInput();
		if (!startScreen->IsActive())
		{
			waveManager.StartFirstWave();
			enemies.clear();
			bullets.clear();
			currentState = GameState::Playing;
		}
		Draw(mousePosition);
		continue;
	}

		HandleInput(mousePosition);
		Update(deltaTime, mousePosition);
		Draw(mousePosition);
	}
}

Vector2 Game::GetRandomEnemyPosition() const
{
	float spawnOffset = basicEnemySize / 2.0f + 12.0f;
	int side = GetRandomValue(0, 3);

	if (side == 0)
	{
		return Vector2{(float)GetRandomValue(0, screenWidth), -spawnOffset};
	}
	if (side == 1)
	{
		return Vector2{screenWidth + spawnOffset, (float)GetRandomValue(0, screenHeight)};
	}
	if (side == 2)
	{
		return Vector2{(float)GetRandomValue(0, screenWidth), screenHeight + spawnOffset};
	}

	return Vector2{-spawnOffset, (float)GetRandomValue(0, screenHeight)};
}

Vector2 Game::GetDirection(Vector2 from, Vector2 to) const
{
	Vector2 direction = Vector2Subtract(to, from);
	if (Vector2Length(direction) > 0.0f)
	{
		return Vector2Normalize(direction);
	}

	return Vector2{1.0f, 0.0f};
}

void Game::LoadTextures()
{
	backgroundTexture = LoadTexture("../assets/images/background.png");
	playerTexture = LoadTexture("../assets/images/player.png");
	bulletTexture = LoadTexture("../assets/images/bullet.png");
	basicEnemyTexture = LoadTexture("../assets/images/basic_enemy.png");
	fastEnemyTexture = LoadTexture("../assets/images/fast_enemy.png");
	tankEnemyTexture = LoadTexture("../assets/images/tank_enemy.png");
	bossTexture = LoadTexture("../assets/images/boss.png");
	shopTexture = LoadTexture("../assets/images/shop.png");
	medkitTexture = LoadTexture("../assets/images/medkit.png");
	bulletMagTexture = LoadTexture("../assets/images/bullet_mag.png");
	startTexture = LoadTexture("../assets/images/start.png");
	victoryTexture = LoadTexture("../assets/images/victory.png");
	gameOverTexture = LoadTexture("../assets/images/game_over.png");
	restartTexture = LoadTexture("../assets/images/restart.png");
	levelTextures[0] = LoadTexture("../assets/images/level_1.png");
	levelTextures[1] = LoadTexture("../assets/images/level_2.png");
	levelTextures[2] = LoadTexture("../assets/images/level_3.png");
	levelTextures[3] = LoadTexture("../assets/images/level_4.png");
	levelTextures[4] = LoadTexture("../assets/images/level_5.png");
	levelTextures[5] = LoadTexture("../assets/images/level_6.png");
	levelTextures[6] = LoadTexture("../assets/images/level_7.png");
	levelTextures[7] = LoadTexture("../assets/images/level_8.png");
	levelTextures[8] = LoadTexture("../assets/images/level_9.png");
	levelTextures[9] = LoadTexture("../assets/images/level_boss.png");
	soundtrack = LoadMusicStream("../assets/sounds/soundtrack.mp3");
	soundtrackLoaded = soundtrack.stream.buffer != nullptr;
	if (soundtrackLoaded)
	{
		soundtrack.looping = true;
		SetMusicVolume(soundtrack, 0.45f);
		PlayMusicStream(soundtrack);
	}
}

void Game::UnloadTextures()
{
	for (int i = 0; i < 10; i++)
	{
		UnloadTexture(levelTextures[i]);
	}
	UnloadTexture(victoryTexture);
	UnloadTexture(gameOverTexture);
	UnloadTexture(restartTexture);
	UnloadTexture(startTexture);
	UnloadTexture(bossTexture);
	UnloadTexture(tankEnemyTexture);
	UnloadTexture(fastEnemyTexture);
	UnloadTexture(basicEnemyTexture);
	UnloadTexture(shopTexture);
	UnloadTexture(medkitTexture);
	UnloadTexture(bulletMagTexture);
	UnloadTexture(bulletTexture);
	UnloadTexture(playerTexture);
	UnloadTexture(backgroundTexture);
}

void Game::CreateGameObjects()
{
	const float playerSpeed = Constants::PLAYER_SPEED;
	const float playerRotationOffset = Constants::PLAYER_ROTATION_OFFSET;
	const float shootCooldown = Constants::SHOOT_COOLDOWN;
	const int playerMaxHealth = Constants::PLAYER_MAX_HEALTH;

	player = std::make_unique<Player>(&playerTexture, playerStartPosition, playerSize, playerSpeed, playerRotationOffset, shootCooldown, playerMaxHealth);
	shop = std::make_unique<Shop>(&shopTexture, &medkitTexture, &bulletMagTexture, screenWidth);
	startScreen = std::make_unique<StartScreen>(&startTexture, screenWidth, screenHeight);
	waveManager = WaveManager(
		{
			&levelTextures[0],
			&levelTextures[1],
			&levelTextures[2],
			&levelTextures[3],
			&levelTextures[4],
			&levelTextures[5],
			&levelTextures[6],
			&levelTextures[7],
			&levelTextures[9],
		},
		basicEnemyCount,
		screenWidth,
		screenHeight);

	enemies.clear();
}

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

void Game::ResetGame()
{
	player->Reset(playerStartPosition);
	bullets.clear();
	enemies.clear();
	enemiesRemainingInWave = 0;
	coins = 0;
	UpdateHighScore();
	currentState = GameState::Playing;
	waveManager.StartFirstWave();
}

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
