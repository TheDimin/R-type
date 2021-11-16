#include "../precomp.h"
#include "FSM.h"
#include "Event.h"
#include "State.h"
#include "Transition.h"

void FSM::InvokeEvent(const Event& e)
{
	for (int i = 0; i < transitions.Size(); ++i)
	{
		Transition* element = transitions[i];
		if (element->SourceState != nullptr)
			if (element->SourceState != activeState)
				continue;

		if (element->TransitionGuard(e))
		{
			//Prevent infinite loop when triggering new state on state exit
			State* OldState = activeState;
			activeState = element->TargetState;

			OldState->OnStateExit(e);
			element->TransitionAction(e);
			activeState->OnStateEnter(e);
#ifdef STATE_DEBUG
			printf("Switched states oldState:'%s' newState:'%s'\n", typeid(*OldState).name(), typeid(*activeState).name());
#endif

			return;
		}
#ifdef STATE_DEBUG
		printf("Failed to find new state :\nCurrentState: '%s'\nEvent: '%s'\n", typeid(*activeState).name(), typeid(e).name());
#endif
	}
}

void FSM::RegisterTransition(Transition* transition)
{
	transitions.Add(transition);

	if (transition->SourceState != nullptr)
		if (!states.Contains(transition->SourceState))
		{
			RegisterState(transition->SourceState);
		}

	if (!states.Contains(transition->TargetState))
	{
		RegisterState(transition->TargetState);
	}
}

void FSM::RegisterTransition(State* sourceState, State* targetState, EventAction* Guard, EventAction* Action)
{
	RegisterTransition(new Transition(sourceState, targetState, Guard, Action));
}

void FSM::ActivateFsm(State* state)
{
	assert(state != nullptr && "active state is a nullptr");
	assert(activeState == nullptr && "StateMachine already activated use a event to trigger transistion");

#ifdef STATE_DEBUG
	printf("Activated StateMachine with state: %s \n", typeid(state).name());
#endif

	state->OnStateEnter(Event());
	activeState = state;
}

void FSM::ResetFsm(State* state)
{
#ifdef STATE_DEBUG
	printf("Reset StateMachine with state: %s \n", typeid(state).name());
#endif
	if (activeState != nullptr)
		activeState->OnStateExit(Event());

	activeState = state;
	activeState->OnStateEnter(Event());
}

void FSM::RegisterState(State* state)
{
	states.Add(state);
	OnStateRegister(state);
}

void FSM::Tick(float DeltaTime)
{
	if (activeState != nullptr)
		activeState->Tick(DeltaTime);
}

void FSM::Render(Tmpl8::Surface* Surface) const
{
	if (activeState != nullptr)
		activeState->Render(Surface);
}