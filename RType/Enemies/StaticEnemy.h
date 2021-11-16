#pragma once
#include "../Extension/Entity.h"
class StaticEnemy :
	public Entity
{
public:
	StaticEnemy();
	void Tick(float DeltaTime) override;
protected:
	void OnCollide(ICollider* self, ICollider* other) override;
};