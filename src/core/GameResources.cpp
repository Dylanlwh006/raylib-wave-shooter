#include "Game.h"

// Loads and releases the textures and music owned by Game.
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
