#pragma once

#include "raylib.h"

// Common transform, texture, drawing, and collision data for game objects.
class Entity
{
protected:
	Vector2 position;
	float size;
	float rotation;
	Texture2D *texture;

public:
	Entity(Texture2D *entityTexture, Vector2 startPosition, float entitySize)
		: position(startPosition), size(entitySize), rotation(0.0f), texture(entityTexture)
	{
	}

	virtual ~Entity() = default;

	virtual void Update(float deltaTime)
	{
		(void)deltaTime;
	}

	virtual void Draw() const
	{
		if (texture == nullptr)
		{
			return;
		}

		DrawTexturePro(
			*texture,
			{0.0f, 0.0f, (float)texture->width, (float)texture->height},
			{position.x, position.y, size, size},
			{size / 2.0f, size / 2.0f},
			rotation,
			WHITE);
	}

	virtual Rectangle GetHitbox() const
	{
		return {
			position.x - size / 2.0f,
			position.y - size / 2.0f,
			size,
			size};
	}

	Vector2 GetPosition() const
	{
		return position;
	}

	float GetSize() const
	{
		return size;
	}

	float GetRotation() const
	{
		return rotation;
	}

	void SetPosition(Vector2 newPosition)
	{
		position = newPosition;
	}

	void SetRotation(float newRotation)
	{
		rotation = newRotation;
	}
};
