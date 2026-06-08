#pragma once

#include "entities/Player.h"
#include "raylib.h"
#include "ui/Button.h"

// Handles the shop overlay, purchases, and player upgrade effects.
class Shop
{
private:
	Texture2D *shopTexture;
	Texture2D *medkitTexture;
	Texture2D *bulletMagTexture;

	Rectangle shopIcon;
	Rectangle shopPanel;
	Rectangle medkitItem;
	Rectangle bulletMagItem;
	Button closeButton;

	bool open;
	int medkitCost;
	int medkitHealAmount;
	int bulletMagCost;
	float bulletMagSpeedBoost;

public:
	Shop(Texture2D *shopIconTexture, Texture2D *medkitIconTexture, Texture2D *bulletMagIconTexture, int screenWidth);

	void HandleInput(Vector2 mousePosition, int &coins, Player &player);
	void Draw(Vector2 mousePosition, int coins) const;

	bool IsOpen() const;
};
