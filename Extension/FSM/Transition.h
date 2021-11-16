#pragma once

class State;
class Event;

class EventAction
{
public:
	EventAction() = default;
	virtual ~EventAction() = default;
	virtual bool Run(const Event&) = 0;

	bool operator() (const Event& e)
	{
		return Run(e);
	}
};

class Transition
{
public:
	Transition(State* sourceState, State* targetState, EventAction* Guard = nullptr, EventAction* Action = nullptr)
		: SourceState(sourceState),
		TargetState(targetState)
	{
		guardLambda = Guard;
		actionLambda = Action;
	}

	~Transition()
	{
		delete guardLambda;
		delete actionLambda;

		guardLambda = nullptr;
		actionLambda = nullptr;
	}

	State* SourceState = nullptr;
	State* TargetState = nullptr;


	EventAction* guardLambda = nullptr;
	EventAction* actionLambda = nullptr;

	virtual bool TransitionGuard(const Event& event)
	{
		if (guardLambda == nullptr)
			return true;

		return guardLambda->Run(event);
	}

	virtual void TransitionAction(const Event& event)
	{
		if (actionLambda == nullptr) return;
		actionLambda->Run(event);
	}
};