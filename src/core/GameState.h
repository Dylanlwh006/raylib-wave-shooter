#pragma once

// Top-level screens used to keep menu and gameplay behavior separate.
enum class GameState
{
	StartScreen,
	Loading,
	Playing,
	Paused,
	GameOver,
	Victory
};
