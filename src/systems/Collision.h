#pragma once

#include "entities/Enemy.h"
#include "entities/Player.h"

class Collision
{
public:
	static bool CheckPlayerEnemyCollision(const Player &player, const Enemy &enemy);
};
