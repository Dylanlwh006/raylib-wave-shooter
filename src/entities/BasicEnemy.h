#pragma once

#include "Enemy.h"
#include "raylib.h"

class BasicEnemy : public Enemy
{
private:
	float rotationOffset;

public:
	BasicEnemy(Texture2D *enemyTexture, Vector2 startPosition, float enemySize, float enemySpeed, float enemyHitboxScale, int enemyHealth, int enemyAttackDamage, float enemyRotationOffset);

	void Update(Vector2 playerPosition, float deltaTime) override;
};
