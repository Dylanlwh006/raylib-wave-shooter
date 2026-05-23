#pragma once

namespace Constants
{
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;

	constexpr float PLAYER_SIZE = 96.0f;
	constexpr float PLAYER_SPEED = 280.0f;
	constexpr float PLAYER_ROTATION_OFFSET = 283.0f;
	constexpr int PLAYER_MAX_HEALTH = 100;

	constexpr float BULLET_SPEED = 650.0f;
	constexpr float BULLET_SIZE = 26.0f;
	constexpr float BULLET_ROTATION_OFFSET = 90.0f;
	constexpr int BULLET_DAMAGE = 1;
	constexpr float SHOOT_COOLDOWN = 0.35f;

	constexpr float ENEMY_SIZE_MULTIPLIER = 1.4f;
	constexpr float ENEMY_HITBOX_SCALE = 0.68f;
	constexpr float ENEMY_ROTATION_OFFSET = -90.0f;
	constexpr float BASIC_ENEMY_SPEED = 120.0f;
	constexpr int BASIC_ENEMY_HEALTH = 5;
	constexpr int BASIC_ENEMY_DAMAGE = 20;

	constexpr float FAST_ENEMY_SPEED_MULTIPLIER = 1.5f;
	constexpr int FAST_ENEMY_HEALTH = 3;

	constexpr float TANK_ENEMY_SPEED_MULTIPLIER = 0.5f;
	constexpr int TANK_ENEMY_HEALTH = 8;

	constexpr float BOSS_SIZE_MULTIPLIER = 1.35f;
	constexpr float BOSS_SPEED = 90.0f;
	constexpr int BOSS_HEALTH = 300;
	constexpr int BOSS_DAMAGE = 50;

	constexpr int LEVEL_COUNT = 10;
	constexpr int BOSS_LEVEL = 10;
	constexpr int MAX_ACTIVE_ENEMIES = 2;
	constexpr int ENEMIES_PER_WAVE = 5;
	constexpr int COINS_PER_KILL = 50;
}
