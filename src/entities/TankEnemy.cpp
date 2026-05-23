#include "TankEnemy.h"

TankEnemy::TankEnemy(Texture2D *enemyTexture, Vector2 startPosition, float enemySize, float enemySpeed, float enemyHitboxScale, int enemyHealth, int enemyAttackDamage, float enemyRotationOffset)
	: Enemy(enemyTexture, startPosition, enemySize, enemySpeed, enemyHitboxScale, enemyHealth, enemyAttackDamage),
	  rotationOffset(enemyRotationOffset)
{
}

void TankEnemy::Update(Vector2 playerPosition, float deltaTime)
{
	MoveToward(playerPosition, deltaTime);
	FaceBottomToward(playerPosition, rotationOffset);
}

bool TankEnemy::IsTank() const
{
	return true;
}
