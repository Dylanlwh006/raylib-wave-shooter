#include "Button.h"

#include "ui/FantasyText.h"

Button::Button(Rectangle buttonBounds, const char *buttonLabel, int buttonFontSize, Color buttonColor, Color buttonHoverColor, Color buttonTextColor, Color buttonBorderColor)
	: bounds(buttonBounds),
	  label(buttonLabel),
	  fontSize(buttonFontSize),
	  normalColor(buttonColor),
	  hoverColor(buttonHoverColor),
	  textColor(buttonTextColor),
	  borderColor(buttonBorderColor)
{
}

bool Button::IsHovered(Vector2 mousePosition) const
{
	return CheckCollisionPointRec(mousePosition, bounds);
}

bool Button::IsClicked(Vector2 mousePosition) const
{
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsHovered(mousePosition);
}

void Button::Draw(Vector2 mousePosition) const
{
	Color fillColor = IsHovered(mousePosition) ? hoverColor : normalColor;
	int textWidth = MeasureFantasyText(label, fontSize);

	DrawRectangleRec(bounds, fillColor);
	DrawRectangleLinesEx(bounds, 2.0f, borderColor);
	DrawFantasyText(
		label,
		(int)(bounds.x + bounds.width / 2.0f - textWidth / 2.0f),
		(int)(bounds.y + bounds.height / 2.0f - fontSize / 2.0f),
		fontSize,
		textColor);
}

Rectangle Button::GetBounds() const
{
	return bounds;
}
