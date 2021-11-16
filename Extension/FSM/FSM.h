#pragma once
#include "../Interfaces/ITick.h"
#include "../Interfaces/IRender.h"
#include "../Helpers/DUniquePtr.h"
#include "../Helpers/DVector.h"

class EventAction;
class Event;
class State;

namespace Tmpl8 {
	class Surface;
}

class Transition;

//template<typename Transition, typename Event, typename State>
class FSM : IRender, ITick
{
public:
	virtual void RegisterTransition(Transition* transition);
	virtual void RegisterTransition(State* sourceState, State* targetState, EventAction* Guard = nullptr, EventAction* Action = nullptr);

	void ActivateFsm(State* state);
	/// <summary>
	/// Force FSM into specific state, usefull for objects in pools
	/// </summary>
	/// <param name="state">new active state</param>
	void ResetFsm(State* state);
	void InvokeEvent(const Event& e);

	State* GetState() const { return activeState; }

protected:
	virtual void OnStateRegister(State* state) {}

	void RegisterState(State* state);
public:
	void Tick(float DeltaTime)override;
	void Render(Tmpl8::Surface* Surface) const override;

public:
	DVector<DUniquePtr<Transition>> transitions = DVector<DUniquePtr<Transition>>();
	DVector<DUniquePtr<State>> states = DVector<DUniquePtr<State>>();
protected:
	State* activeState = nullptr;
};