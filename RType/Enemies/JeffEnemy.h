#pragma once
#include "../Extension/Entity.h"
#include "FSM/EntityFSM.h"

class WeaponAttackStage : public EntityStateBase
{
public:
	WeaponAttackStage(unsigned short WeaponID);
	void OnStateEnter(const Event& e) override;

	void Tick(float deltaTime) override;
	unsigned short weaponIndex = 0;
	const Tmpl8::vec2* playerPos = nullptr;

	float moveCooldown = 0;
};

class MoveToPlayer : public EntityStateBase
{
public:

	void Tick(float DeltaTime) override;
	void OnStateEnter(const Event& e) override;
	vec2 targetPos;
	vec2 ownerStartPos;
	float maxDuration = 4;
	float currentDuration = 0;
};

class DelayState :public EntityStateBase
{
	void Tick(float DeltaTime) override;
	void OnStateEnter(const Event& e) override;

	float cooldown = 0;
};

class BossStateProgress :public EntityStateBase
{
public:
	void Tick(float DeltaTime) override;
	void OnStateEnter(const Event& e) override;

	float cooldown = 0;
};

class MoveToAttackPos :public MoveToPlayer
{
public:
	void OnStateEnter(const Event& e) override;
};

class JeffEnemy :
	public EntityWFSM
{
public:
	JeffEnemy();

protected:
	void OnEnable() override;
public:
	int stage = 0;
};