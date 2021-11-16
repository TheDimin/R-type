#pragma once
#include "EntityWFSM.h"
#include "../Extension/Interfaces/IHealth.h"
#include "FSM/EntityFSM.h"

class Spline;

class SplineMove : public EntityStateBase
{
public:
	SplineMove();
	void Tick(float deltaTime) override;
	void OnStateEnter(const Event& e) override;
	DUniquePtr<Spline> spline;
	//const float duration;
	//float currentDuration;
};

class SnakeEnemy :
	public EntityWFSM, public IHealth
{
public:
	SnakeEnemy();
};