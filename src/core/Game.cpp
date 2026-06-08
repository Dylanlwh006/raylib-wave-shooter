#include "Game.h"

#include "core/Constants.h"
#include "raymath.h"
#include "resource_dir.h"

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
			&levelTextures[8],
			&levelTextures[9],
		},
		basicEnemyCount,
		screenWidth,
		screenHeight);

	enemies.clear();
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
