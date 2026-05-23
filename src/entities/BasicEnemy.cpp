#include "BasicEnemy.h"

BasicEnemy::BasicEnemy(Texture2D *enemyTexture, Vector2 startPosition, float enemySize, float enemySpeed, float enemyHitboxScale, int enemyHealth, int enemyAttackDamage, float enemyRotationOffset)
	: Enemy(enemyTexture, startPosition, enemySize, enemySpeed, enemyHitboxScale, enemyHealth, enemyAttackDamage),
	  rotationOffset(enemyRotationOffset)
{
}

void BasicEnemy::Update(Vector2 playerPosition, float deltaTime)
{
	MoveToward(playerPosition, deltaTime);
	FaceBottomToward(playerPosition, rotationOffset);
}
