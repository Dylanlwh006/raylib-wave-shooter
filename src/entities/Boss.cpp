#include "Boss.h"

Boss::Boss(Texture2D *bossTexture, Vector2 startPosition, float bossSize, float bossSpeed, float bossHitboxScale, int bossHealth, int bossAttackDamage, float bossRotationOffset)
	: Enemy(bossTexture, startPosition, bossSize, bossSpeed, bossHitboxScale, bossHealth, bossAttackDamage),
	  rotationOffset(bossRotationOffset)
{
}

void Boss::Update(Vector2 playerPosition, float deltaTime)
{
	MoveToward(playerPosition, deltaTime);
	FaceBottomToward(playerPosition, rotationOffset);
}

bool Boss::IsBoss() const
{
	return true;
}
