#pragma once

#include "Entity.h"
#include "raylib.h"

// Polymorphic enemy base containing shared movement, combat, and hitbox logic.
class Enemy : public Entity
{
protected:
	float speed;
	float hitboxScale;
	int maxHealth;
	int health;
	int attackDamage;

public:
	Enemy(Texture2D *enemyTexture, Vector2 startPosition, float enemySize, float enemySpeed, float enemyHitboxScale, int enemyHealth, int enemyAttackDamage);

	virtual void Update(Vector2 playerPosition, float deltaTime);
	void MoveToward(Vector2 targetPosition, float deltaTime);
	void FaceBottomToward(Vector2 targetPosition, float rotationOffset);
	void TakeDamage(int damage);
	void SetHealth(int newHealth);
	void Reset(Vector2 newPosition);

	bool IsDead() const;
	virtual bool IsBoss() const;
	virtual bool IsFast() const;
	virtual bool IsTank() const;
	int GetHealth() const;
	int GetAttackDamage() const;
	Rectangle GetHitbox() const override;
};
