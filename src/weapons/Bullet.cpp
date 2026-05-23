#include "Bullet.h"

#include "raymath.h"

Bullet::Bullet(Texture2D *bulletTexture, Vector2 startPosition, Vector2 startVelocity, float bulletSize, float bulletRotation, int bulletDamage)
	: Entity(bulletTexture, startPosition, bulletSize), velocity(startVelocity), damage(bulletDamage)
{
	rotation = bulletRotation;
}

void Bullet::Update(float deltaTime)
{
	position = Vector2Add(position, Vector2Scale(velocity, deltaTime));
}

bool Bullet::IsOffScreen(int screenWidth, int screenHeight) const
{
	return position.x < -size || position.x > screenWidth + size ||
		   position.y < -size || position.y > screenHeight + size;
}

int Bullet::GetDamage() const
{
	return damage;
}
