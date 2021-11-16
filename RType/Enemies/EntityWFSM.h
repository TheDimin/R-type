#pragma once
#include "../Extension/Entity.h"

class EntityStateBase;
class EntityFSM;

class EntityWFSM : public Entity
{
public:
	EntityWFSM();
protected:
	void OnEnable() override;
public:
	void Tick(float DeltaTime) override;
	EntityFSM* GetFSM() const { return fsm; }

protected:
	void OnCollide(ICollider* self, ICollider* other) override;
	DUniquePtr<EntityFSM> fsm = nullptr;
	EntityStateBase* startState = nullptr;
};
