#pragma once
#include "EntityFSM.h"
class StateBlockGuard : EnemyAction
{
public:
	StateBlockGuard(DVector<State*> states) : blockingStates(states) {}

	bool Run(Event* e) override
	{
		const State* currentState = fsm->GetState();
		for (int i = 0; i < blockingStates.Size(); ++i)
		{
			if (currentState == blockingStates[i])
				return false;
		}
		return true;
	}

	DVector<State* > blockingStates;
};