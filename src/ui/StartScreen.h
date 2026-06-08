#pragma once

#include "raylib.h"

// Displays the opening artwork until the user provides keyboard or mouse input.
class StartScreen
{
private:
	Texture2D *backgroundTexture;
	bool active;
	int screenWidth;
	int screenHeight;

public:
	StartScreen(Texture2D *startBackgroundTexture, int windowWidth, int windowHeight);

	void HandleInput();
	void Draw() const;

	bool IsActive() const;
};
