#include "../precomp.h"
#include "EntityKilledState.h"

void EntityKilledState::Tick(float DeltaTime)
{
	EntityStateBase::Tick(DeltaTime);

	//
	timer += DeltaTime;

	if (timer >= KillTimer)
		owner->OnKilled();
}

void EntityKilledState::OnStateEnter(const Event& E)
{
	EntityStateBase::OnStateEnter(E);
	timer = 0;
}
