#pragma once

#include "raylib.h"

#include <cstring>

inline int MeasureFantasyText(const char *text, int fontSize)
{
	return MeasureText(text, fontSize);
}

inline void DrawFantasyText(const char *text, int x, int y, int fontSize, Color accentColor)
{
	Color shadow = Fade(BLACK, 0.78f);
	Color metalEdge = Color{204, 186, 132, 255};
	Color darkEdge = Color{45, 36, 34, 255};
	Color stoneFill = Color{178, 174, 163, 255};
	Color blood = Color{118, 8, 14, 235};

	DrawText(text, x + 4, y + 5, fontSize, shadow);
	DrawText(text, x - 2, y, fontSize, darkEdge);
	DrawText(text, x + 2, y, fontSize, darkEdge);
	DrawText(text, x, y - 2, fontSize, metalEdge);
	DrawText(text, x, y + 2, fontSize, darkEdge);
	DrawText(text, x, y, fontSize, stoneFill);
	DrawText(text, x, y, fontSize, Fade(accentColor, 0.22f));

	int width = MeasureText(text, fontSize);
	int crackCount = width / 54 + 1;
	for (int i = 0; i < crackCount; i++)
	{
		int crackX = x + 12 + i * 49;
		int crackY = y + 5 + (i % 3) * (fontSize / 5);
		DrawLine(crackX, crackY, crackX + 10, crackY + fontSize / 3, Fade(BLACK, 0.44f));
		DrawLine(crackX + 2, crackY + fontSize / 4, crackX + 14, crackY + fontSize / 4 + 3, Fade(BLACK, 0.30f));
	}

	int textLength = (int)std::strlen(text);
	if (textLength > 0 && fontSize >= 18)
	{
		int stainX = x + (width * 3) / 4;
		int stainY = y + fontSize / 4;
		DrawCircle(stainX, stainY, (float)fontSize * 0.09f, blood);
		DrawCircle(stainX + fontSize / 5, stainY + fontSize / 6, (float)fontSize * 0.055f, Fade(blood, 0.82f));
		DrawLine(stainX, stainY + fontSize / 8, stainX - fontSize / 7, y + fontSize - 2, Fade(blood, 0.68f));
	}
}
