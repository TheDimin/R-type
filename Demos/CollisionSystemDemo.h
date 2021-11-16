#pragma once
#include "../Extension/Level.h"

class TileCollider;

class CollisionSystemDemo : public Level
{
protected:
	void OnLoad() override;

	//We own the collider
	DUniquePtr<TileCollider> tileCollider;
};
