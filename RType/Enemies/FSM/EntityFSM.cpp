#include "../precomp.h"
#include "EntityFSM.h"

void EntityFSM::OnStateRegister(State* state)
{

	FSM::OnStateRegister(state);
	if (EntityStateBase* eState = dynamic_cast<EntityStateBase*>(state))
		eState->owner = owner;
}
