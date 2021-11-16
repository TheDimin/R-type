#pragma once
#include "../RType/Enemies/FSM/EntityFSM.h"

class EntityKilledState : public EntityStateBase
{
public:
	void Tick(float deltaTime) override;
	void OnStateEnter(const Event& e) override;

	float timer = 0;
	float KillTimer = .1f;
};