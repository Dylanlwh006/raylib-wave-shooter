#pragma once

#include "Enemy.h"
#include "raylib.h"

// Low-health enemy variant that trades durability for movement speed.
class FastEnemy : public Enemy
{
private:
	float rotationOffset;

public:
	FastEnemy(Texture2D *enemyTexture, Vector2 startPosition, float enemySize, float enemySpeed, float enemyHitboxScale, int enemyHealth, int enemyAttackDamage, float enemyRotationOffset);

	void Update(Vector2 playerPosition, float deltaTime) override;
	bool IsFast() const override;
};
