#pragma once

#include "entities/Enemy.h"
#include "entities/Player.h"

// Centralizes collision queries used by the game controller.
class Collision
{
public:
	static bool CheckPlayerEnemyCollision(const Player &player, const Enemy &enemy);
};
