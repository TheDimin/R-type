#include "../precomp.h"
#include "EntityWFSM.h"
#include "../Extension/Modules/Bullet/Projectile.h"
#include "FSM/EnemyEvents.h"
#include "FSM/EntityFSM.h"

EntityWFSM::EntityWFSM()
{
	fsm.Set(new EntityFSM(this));
}

void EntityWFSM::OnEnable()
{
	Entity::OnEnable();
	fsm->ResetFsm(startState);
}

void EntityWFSM::Tick(float DeltaTime)
{
	Entity::Tick(DeltaTime);
	fsm->Tick(DeltaTime);
}

void EntityWFSM::OnCollide(ICollider* self, ICollider* other)
{
	if (dynamic_cast<Projectile*>(other->GetOwner()))
		fsm->InvokeEvent(CollideEvent(other->GetOwner()));
}