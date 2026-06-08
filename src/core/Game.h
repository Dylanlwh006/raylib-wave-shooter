#pragma once

#include "core/GameState.h"
#include "entities/BasicEnemy.h"
#include "entities/Boss.h"
#include "entities/Enemy.h"
#include "entities/FastEnemy.h"
#include "entities/Player.h"
#include "entities/TankEnemy.h"
#include "raylib.h"
#include "systems/SaveManager.h"
#include "systems/Shop.h"
#include "systems/WaveManager.h"
#include "ui/Button.h"
#include "ui/HUD.h"
#include "ui/StartScreen.h"
#include "weapons/Bullet.h"

#include <memory>
#include <vector>

// Owns resources and coordinates input, state changes, gameplay, saving, and drawing.
class Game
{
private:
	const int screenWidth;
	const int screenHeight;

	Texture2D backgroundTexture;
	Texture2D playerTexture;
	Texture2D bulletTexture;
	Texture2D basicEnemyTexture;
	Texture2D fastEnemyTexture;
	Texture2D tankEnemyTexture;
	Texture2D bossTexture;
	Texture2D shopTexture;
	Texture2D medkitTexture;
	Texture2D bulletMagTexture;
	Texture2D startTexture;
	Texture2D victoryTexture;
	Texture2D gameOverTexture;
	Texture2D restartTexture;
	Texture2D levelTextures[10];
	Music soundtrack;

	std::unique_ptr<Player> player;
	std::unique_ptr<Shop> shop;
	std::unique_ptr<StartScreen> startScreen;
	WaveManager waveManager;
	HUD hud;
	std::vector<Bullet> bullets;
	std::vector<std::unique_ptr<Enemy>> enemies;
	GameState currentState;
	GameState previousState;

	Vector2 playerStartPosition;
	Button restartButton;
	Button resumeButton;
	Button saveButton;
	Button loadButton;

	float playerSize;
	float bulletSpeed;
	float bulletSize;
	float bulletRotationOffset;
	float basicEnemySize;
	float basicEnemySpeed;
	float basicEnemyHitboxScale;
	float basicEnemyRotationOffset;
	float fastEnemySpeed;
	float tankEnemySpeed;
	float bossSize;
	float bossSpeed;

	int bulletDamage;
	int basicEnemyMaxHealth;
	int fastEnemyMaxHealth;
	int tankEnemyMaxHealth;
	int basicEnemyAttackDamage;
	int bossMaxHealth;
	int bossAttackDamage;
	int basicEnemyCount;
	int enemiesRemainingInWave;
	int coinsPerKill;
	int coins;
	const char *saveFilePath;
	const char *highScoreFilePath;
	int highScore;
	bool soundtrackLoaded;

	Vector2 GetRandomEnemyPosition() const;
	Vector2 GetDirection(Vector2 from, Vector2 to) const;

	void LoadTextures();
	void UnloadTextures();
	void CreateGameObjects();
	void ResetGame();
	void SpawnWave();
	void SpawnEnemy();
	void SpawnEnemiesToActiveLimit();
	void SpawnBoss();
	bool IsBossWave() const;
	void TogglePause();
	void SaveGame();
	void LoadGame();
	void UpdateHighScore();
	void HandleInput(Vector2 mousePosition);
	void Update(float deltaTime, Vector2 mousePosition);
	void UpdateBullets(float deltaTime);
	void CheckBulletEnemyCollisions();
	void CheckPlayerEnemyCollisions();
	void Draw(Vector2 mousePosition) const;
	void DrawGameOverScreen(Vector2 mousePosition) const;
	void DrawPauseScreen(Vector2 mousePosition) const;
	void DrawVictoryScreen() const;
	void DrawEndScreenScores() const;

public:
	Game();
	~Game();

	void run();
};
