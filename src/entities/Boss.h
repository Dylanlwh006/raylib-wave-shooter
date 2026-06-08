#pragma once

#include "Enemy.h"
#include "raylib.h"

// Final-wave enemy identified separately so its death can trigger victory.
class Boss : public Enemy
{
private:
	float rotationOffset;

public:
	Boss(Texture2D *bossTexture, Vector2 startPosition, float bossSize, float bossSpeed, float bossHitboxScale, int bossHealth, int bossAttackDamage, float bossRotationOffset);

	void Update(Vector2 playerPosition, float deltaTime) override;
	bool IsBoss() const override;
};
