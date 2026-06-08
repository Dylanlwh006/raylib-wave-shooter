#pragma once

#include "raylib.h"

// Reusable mouse-driven button with hover styling and centered text.
class Button
{
private:
	Rectangle bounds;
	const char *label;
	int fontSize;
	Color normalColor;
	Color hoverColor;
	Color textColor;
	Color borderColor;

public:
	Button(Rectangle buttonBounds, const char *buttonLabel, int buttonFontSize, Color buttonColor, Color buttonHoverColor, Color buttonTextColor, Color buttonBorderColor);

	bool IsHovered(Vector2 mousePosition) const;
	bool IsClicked(Vector2 mousePosition) const;
	void Draw(Vector2 mousePosition) const;

	Rectangle GetBounds() const;
};
