#include "StartScreen.h"

StartScreen::StartScreen(Texture2D *startBackgroundTexture, int windowWidth, int windowHeight)
	: backgroundTexture(startBackgroundTexture),
	  active(true),
	  screenWidth(windowWidth),
	  screenHeight(windowHeight)
{
}

void StartScreen::HandleInput()
{
	if (active && (GetKeyPressed() != 0 || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)))
	{
		active = false;
	}
}

void StartScreen::Draw() const
{
	DrawTexturePro(
		*backgroundTexture,
		{0.0f, 0.0f, (float)backgroundTexture->width, (float)backgroundTexture->height},
		{0.0f, 0.0f, (float)screenWidth, (float)screenHeight},
		{0.0f, 0.0f},
		0.0f,
		WHITE);
}

bool StartScreen::IsActive() const
{
	return active;
}
