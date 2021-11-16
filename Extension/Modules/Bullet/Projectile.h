#pragma once
#include "../Extension/GameObject.h"
#include "../Extension/Interfaces/ICollision.h"

class CollisionModule;
class WeaponDefinition;


class Projectile final :
	public GameObject
{
public:
	Projectile();
	void Tick(float DeltaTime) override;
	void Awake() override;

protected:
	void OnCollide(ICollider* self, ICollider* other) override;
public:
	Tmpl8::vec2 dir = { 0,0 };
	vec2 velocity = { 0,0 };
	WeaponDefinition* bulletType = nullptr;
	int hitCount = 0;
	GameObject* instigator = nullptr;

	void SetCollisionMask(ECollisionMask collT) { GetCollider()->mask = collT; }
};
