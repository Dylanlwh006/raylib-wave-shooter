#include "Collision.h"

#include "raylib.h"

bool Collision::CheckPlayerEnemyCollision(const Player &player, const Enemy &enemy)
{
	return CheckCollisionRecs(player.GetHitbox(), enemy.GetHitbox());
}
