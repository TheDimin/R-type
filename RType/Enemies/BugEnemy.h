#pragma once
#include "../Enemies/EntityWFSM.h"

#include "FSM/EntityFSM.h"

class WaveMove : public EntityStateBase
{
public:
	void Tick(float deltaTime) override;
	void OnStateEnter(const Event& e) override;
	float startPosY = 0;
};

class StrafeToPos : public EntityStateBase
{
public:
	void Tick(float deltaTime) override;
	void OnStateEnter(const Event& e) override;
	vec2 targetPos{};
};

class BugEnemy :
	public EntityWFSM
{
public:
	BugEnemy();
	~BugEnemy()
	{
#ifdef OBJECT_DESTRUCTOR
		printf("BUG DELETED \n");
#endif
	}
};
