#include "Shop.h"

#include "ui/FantasyText.h"

Shop::Shop(Texture2D *shopIconTexture, Texture2D *medkitIconTexture, Texture2D *bulletMagIconTexture, int screenWidth)
	: shopTexture(shopIconTexture),
	  medkitTexture(medkitIconTexture),
	  bulletMagTexture(bulletMagIconTexture),
	  shopIcon({screenWidth - 52.0f - 16.0f, 14.0f, 52.0f, 52.0f}),
	  shopPanel({150.0f, 105.0f, 500.0f, 365.0f}),
	  medkitItem({190.0f, 180.0f, 420.0f, 96.0f}),
	  bulletMagItem({190.0f, 310.0f, 420.0f, 96.0f}),
	  closeButton({150.0f + 500.0f - 42.0f, 105.0f + 12.0f, 28.0f, 28.0f}, "X", 22, RED, MAROON, WHITE, WHITE),
	  open(false),
	  medkitCost(50),
	  medkitHealAmount(50),
	  bulletMagCost(100),
	  bulletMagSpeedBoost(0.20f)
{
}

void Shop::HandleInput(Vector2 mousePosition, int &coins, Player &player)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, shopIcon))
	{
		open = !open;
	}

	if (!open || !IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		return;
	}

	if (closeButton.IsClicked(mousePosition))
	{
		open = false;
	}
	else if (CheckCollisionPointRec(mousePosition, medkitItem) && coins >= medkitCost)
	{
		player.Heal(medkitHealAmount);
		coins -= medkitCost;
	}
	else if (CheckCollisionPointRec(mousePosition, bulletMagItem) && coins >= bulletMagCost)
	{
		player.IncreaseShootingSpeed(bulletMagSpeedBoost);
		coins -= bulletMagCost;
	}
}

void Shop::Draw(Vector2 mousePosition, int coins) const
{
	Color shopTint = CheckCollisionPointRec(mousePosition, shopIcon) ? WHITE : Fade(WHITE, 0.82f);
	DrawTexturePro(
		*shopTexture,
		{0.0f, 0.0f, (float)shopTexture->width, (float)shopTexture->height},
		shopIcon,
		{0.0f, 0.0f},
		0.0f,
		shopTint);

	if (!open)
	{
		return;
	}

	DrawRectangleRec(shopPanel, Fade(BLACK, 0.86f));
	DrawRectangleLinesEx(shopPanel, 2.0f, GOLD);
	DrawFantasyText("Shop", (int)shopPanel.x + 24, (int)shopPanel.y + 18, 30, GOLD);

	closeButton.Draw(mousePosition);

	Color medkitColor = CheckCollisionPointRec(mousePosition, medkitItem) ? Fade(DARKGREEN, 0.82f) : Fade(DARKGRAY, 0.82f);
	DrawRectangleRec(medkitItem, medkitColor);
	DrawRectangleLinesEx(medkitItem, 1.0f, WHITE);
	DrawTexturePro(
		*medkitTexture,
		{0.0f, 0.0f, (float)medkitTexture->width, (float)medkitTexture->height},
		{medkitItem.x + 18.0f, medkitItem.y + 18.0f, 60.0f, 60.0f},
		{0.0f, 0.0f},
		0.0f,
		WHITE);
	DrawFantasyText("Medkit", (int)medkitItem.x + 96, (int)medkitItem.y + 14, 22, WHITE);
	DrawFantasyText("Restores 50 HP, up to max health.", (int)medkitItem.x + 96, (int)medkitItem.y + 42, 16, LIGHTGRAY);
	DrawFantasyText(TextFormat("Cost: %d coins", medkitCost), (int)medkitItem.x + 96, (int)medkitItem.y + 66, 16, coins >= medkitCost ? GOLD : RED);

	Color bulletMagColor = CheckCollisionPointRec(mousePosition, bulletMagItem) ? Fade(DARKBLUE, 0.82f) : Fade(DARKGRAY, 0.82f);
	DrawRectangleRec(bulletMagItem, bulletMagColor);
	DrawRectangleLinesEx(bulletMagItem, 1.0f, WHITE);
	DrawTexturePro(
		*bulletMagTexture,
		{0.0f, 0.0f, (float)bulletMagTexture->width, (float)bulletMagTexture->height},
		{bulletMagItem.x + 18.0f, bulletMagItem.y + 18.0f, 60.0f, 60.0f},
		{0.0f, 0.0f},
		0.0f,
		WHITE);
	DrawFantasyText("Bullet Magazine", (int)bulletMagItem.x + 96, (int)bulletMagItem.y + 14, 22, WHITE);
	DrawFantasyText("Increases shooting speed by 20%.", (int)bulletMagItem.x + 96, (int)bulletMagItem.y + 42, 16, LIGHTGRAY);
	DrawFantasyText(TextFormat("Cost: %d coins", bulletMagCost), (int)bulletMagItem.x + 96, (int)bulletMagItem.y + 66, 16, coins >= bulletMagCost ? GOLD : RED);
}

bool Shop::IsOpen() const
{
	return open;
}
