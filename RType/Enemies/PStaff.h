#pragma once
#include "EntityWFSM.h"
#include "FSM/EntityFSM.h"

class Event;

class WalkState : public EntityStateBase
{
public:
	void OnStateEnter(const Event& e) override;
	void Tick(float deltaTime) override;
};

class JumpState : public EntityStateBase
{
public:
	void OnStateEnter(const Event& e) override;
	void OnStateExit(const Event& newEvent) override;
	void Tick(float deltaTime) override;

	bool lostGround = false;
	vec2 vel{ 0 };
};

class ShootUpwardsState : public EntityStateBase
{
public:
	void OnStateEnter(const Event& e) override;
	void Tick(float deltaTime) override;
};

class RocketLauncher :public WeaponDefinition
{
public:
	RocketLauncher(Entity& Owner);

	void CalculateNewPos(float& deltaTime, Tmpl8::vec2& pos, Projectile& bullet) const override;
	void OnProjectileSpawned(Projectile& bullet) const override;
	const vec2 gravity{ 0,100 };
};

class PStaff : public EntityWFSM
{
public:
	PStaff();
protected:
	void OnCollide(ICollider* self, ICollider* other) override;

public:
	void Awake() override;
	const float playerDistanceTrashHold = 0.4f;
	void CalculatePhysics(float& DeltaTime) override;

	TileCollider* mapcl = nullptr;
};