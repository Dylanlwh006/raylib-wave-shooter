#include "Enemy.h"

#include "raymath.h"

#include <cmath>

Enemy::Enemy(Texture2D *enemyTexture, Vector2 startPosition, float enemySize, float enemySpeed, float enemyHitboxScale, int enemyHealth, int enemyAttackDamage)
	: Entity(enemyTexture, startPosition, enemySize),
	  speed(enemySpeed),
	  hitboxScale(enemyHitboxScale),
	  maxHealth(enemyHealth),
	  health(enemyHealth),
	  attackDamage(enemyAttackDamage)
{
}

void Enemy::Update(Vector2 playerPosition, float deltaTime)
{
	(void)playerPosition;
	(void)deltaTime;
}

void Enemy::MoveToward(Vector2 targetPosition, float deltaTime)
{
	Vector2 direction = Vector2Subtract(targetPosition, position);
	if (Vector2Length(direction) > 0.0f)
	{
		direction = Vector2Normalize(direction);
		position = Vector2Add(position, Vector2Scale(direction, speed * deltaTime));
	}
}

void Enemy::FaceBottomToward(Vector2 targetPosition, float rotationOffset)
{
	Vector2 direction = Vector2Subtract(targetPosition, position);
	if (Vector2Length(direction) > 0.0f)
	{
		direction = Vector2Normalize(direction);
	}
	else
	{
		direction = {0.0f, 1.0f};
	}

	rotation = atan2f(direction.y, direction.x) * RAD2DEG + rotationOffset;
}

void Enemy::TakeDamage(int damage)
{
	health -= damage;
}

void Enemy::SetHealth(int newHealth)
{
	health = newHealth;
	if (health < 0)
	{
		health = 0;
	}
	if (health > maxHealth)
	{
		health = maxHealth;
	}
}

void Enemy::Reset(Vector2 newPosition)
{
	position = newPosition;
	health = maxHealth;
}

bool Enemy::IsDead() const
{
	return health <= 0;
}

bool Enemy::IsBoss() const
{
	return false;
}

bool Enemy::IsFast() const
{
	return false;
}

bool Enemy::IsTank() const
{
	return false;
}

int Enemy::GetHealth() const
{
	return health;
}

int Enemy::GetAttackDamage() const
{
	return attackDamage;
}

Rectangle Enemy::GetHitbox() const
{
	float hitboxSize = size * hitboxScale;

	return {
		position.x - hitboxSize / 2.0f,
		position.y - hitboxSize / 2.0f,
		hitboxSize,
		hitboxSize};
}
