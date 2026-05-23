#pragma once

#include "entities/Entity.h"
#include "raylib.h"

class Bullet : public Entity
{
private:
	Vector2 velocity;
	int damage;

public:
	Bullet(Texture2D *bulletTexture, Vector2 startPosition, Vector2 startVelocity, float bulletSize, float bulletRotation, int bulletDamage);

	void Update(float deltaTime) override;
	bool IsOffScreen(int screenWidth, int screenHeight) const;
	int GetDamage() const;
};
