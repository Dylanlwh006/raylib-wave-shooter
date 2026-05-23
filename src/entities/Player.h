#pragma once

#include "Entity.h"
#include "raylib.h"

class Player : public Entity
{
private:
	float speed;
	float rotationOffset;
	float shootCooldown;
	float shootTimer;
	int maxHealth;
	int health;

public:
	Player(Texture2D *playerTexture, Vector2 startPosition, float playerSize, float moveSpeed, float aimRotationOffset, float fireCooldown, int startingHealth);

	void UpdateTimers(float deltaTime);
	void HandleMovement(float deltaTime, int screenWidth, int screenHeight);
	void AimAt(Vector2 targetPosition);
	void Heal(int amount);
	void TakeDamage(int damage);
	void IncreaseShootingSpeed(float percent);
	void SetHealth(int newHealth);
	void SetShootCooldown(float newShootCooldown);
	void ResetShootTimer();
	void Reset(Vector2 startPosition);

	bool CanShoot() const;
	bool IsAlive() const;
	int GetHealth() const;
	int GetMaxHealth() const;
	float GetShootCooldown() const;
};
