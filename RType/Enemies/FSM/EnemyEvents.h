#pragma once
#include "EntityFSM.h"

class CollideEvent : public Event
{
public:
	CollideEvent(GameObject* go) : killer(go) {}
	GameObject* killer = nullptr;
};

class MoveComplete :public Event
{

};

class StartAttackEvent :public Event
{
};
class JumpEvent : public Event
{
public:
	JumpEvent() { velocity = vec2(0); }
	JumpEvent(vec2 TargetPos)
	{
		velocity = TargetPos;
	}
	JumpEvent(vec2 Dir, float Dis)
	{
		velocity = Dir.normalized() * Dis;
	}
	vec2 velocity;
};

class LevelEvent : public Event
{
public:
	LevelEvent(Level* LVL) : lvl(LVL) {}
	Level* lvl = nullptr;
};