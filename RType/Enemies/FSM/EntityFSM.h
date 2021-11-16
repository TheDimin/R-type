#pragma once
#include "../Extension/FSM/FSM.h"
#include "../Extension/FSM/Event.h"
#include "../Extension/FSM/Transition.h"
#include "../RType/Enemies/EntityWFSM.h"
#include "../Extension/FSM/State.h"

class EntityWFSM;
class EntityFSM;

template<typename T>
class GuardTypeCheck : public EventAction
{
protected:
	bool Run(const Event& CalledEvent) override
	{
		T* CastAttempt = dynamic_cast<T*>(const_cast<Event*>(&CalledEvent));
		return CastAttempt != nullptr;
	}
};

class EnemyAction : public EventAction
{
protected:
	friend EntityFSM;
	EntityFSM* fsm = nullptr;
};

class EntityStateBase : public State
{
protected:
	friend class EntityFSM;
	EntityWFSM* owner = nullptr;

	Level* GetLevel() const { return owner->GetLevel(); }

	template<typename OwnerType>
	OwnerType* GetOwner()const { return dynamic_cast<OwnerType*>(owner); }

	template<typename ModuleType>
	ModuleType* GetModule() const { return owner->GetLevel()->GetModule<ModuleType>(); }
public:
	~EntityStateBase()
	{
#ifdef OBJECT_DESTRUCTOR
		printf("EntityStateBase released \n");
#endif
	}

	void OnStateEnter(const Event& e) override {}
	void OnStateExit(const Event& newEvent) override {}
	void Render(Tmpl8::Surface* surface) const override {};
};

class EntityFSM :public FSM
{
public:
	EntityFSM(EntityWFSM* Owner)
	{
		owner = Owner;
	}
public:
	EntityWFSM* owner = nullptr;

protected:
	void OnStateRegister(State* state) override;
};