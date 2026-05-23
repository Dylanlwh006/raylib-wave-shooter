#include "Player.h"

#include "raymath.h"

#include <cmath>

Player::Player(Texture2D *playerTexture, Vector2 startPosition, float playerSize, float moveSpeed, float aimRotationOffset, float fireCooldown, int startingHealth)
	: Entity(playerTexture, startPosition, playerSize),
	  speed(moveSpeed),
	  rotationOffset(aimRotationOffset),
	  shootCooldown(fireCooldown),
	  shootTimer(0.0f),
	  maxHealth(startingHealth),
	  health(startingHealth)
{
}

void Player::UpdateTimers(float deltaTime)
{
	if (shootTimer > 0.0f)
	{
		shootTimer -= deltaTime;
	}
}

void Player::HandleMovement(float deltaTime, int screenWidth, int screenHeight)
{
	Vector2 movement = {0.0f, 0.0f};

	if (IsKeyDown(KEY_W))
		movement.y -= 1.0f;
	if (IsKeyDown(KEY_S))
		movement.y += 1.0f;
	if (IsKeyDown(KEY_A))
		movement.x -= 1.0f;
	if (IsKeyDown(KEY_D))
		movement.x += 1.0f;

	if (Vector2Length(movement) > 0.0f)
	{
		movement = Vector2Normalize(movement);
		position = Vector2Add(position, Vector2Scale(movement, speed * deltaTime));
	}

	float radius = size / 2.0f;
	position.x = Clamp(position.x, radius, screenWidth - radius);
	position.y = Clamp(position.y, radius, screenHeight - radius);
}

void Player::AimAt(Vector2 targetPosition)
{
	Vector2 aimDirection = Vector2Subtract(targetPosition, position);
	if (Vector2Length(aimDirection) > 0.0f)
	{
		aimDirection = Vector2Normalize(aimDirection);
	}
	else
	{
		aimDirection = {1.0f, 0.0f};
	}

	rotation = atan2f(aimDirection.y, aimDirection.x) * RAD2DEG + rotationOffset;
}

void Player::Heal(int amount)
{
	health += amount;
	if (health > maxHealth)
	{
		health = maxHealth;
	}
}

void Player::TakeDamage(int damage)
{
	health -= damage;
	if (health < 0)
	{
		health = 0;
	}
}

void Player::IncreaseShootingSpeed(float percent)
{
	shootCooldown *= 1.0f - percent;
	if (shootCooldown < 0.05f)
	{
		shootCooldown = 0.05f;
	}
}

void Player::SetHealth(int newHealth)
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

void Player::SetShootCooldown(float newShootCooldown)
{
	shootCooldown = newShootCooldown;
	if (shootCooldown < 0.05f)
	{
		shootCooldown = 0.05f;
	}
}

void Player::ResetShootTimer()
{
	shootTimer = shootCooldown;
}

void Player::Reset(Vector2 startPosition)
{
	position = startPosition;
	health = maxHealth;
	shootTimer = 0.0f;
}

bool Player::CanShoot() const
{
	return shootTimer <= 0.0f;
}

bool Player::IsAlive() const
{
	return health > 0;
}

int Player::GetHealth() const
{
	return health;
}

int Player::GetMaxHealth() const
{
	return maxHealth;
}

float Player::GetShootCooldown() const
{
	return shootCooldown;
}
