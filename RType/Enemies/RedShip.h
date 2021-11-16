#pragma once
#include "../Extension/Entity.h"

class RedShip :
	public Entity
{
public:
	RedShip();
	~RedShip();
	void Tick(float DeltaTime) override;

protected:
	void OnEnable() override;


	void OnCollide(ICollider* self, ICollider* other) override;
	float timeSinceStart = 0;
	float startY = 0;
	int MoveHeight = 0;
	bool xOdd = false;
};