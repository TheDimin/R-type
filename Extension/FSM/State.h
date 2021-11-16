#pragma once
#include "../Interfaces/ITick.h"
#include "../Interfaces/IRender.h"

namespace Tmpl8 {
	class Surface;
	class vec2;
}

class Event;

class State : ITick, IRender
{
public:
	virtual ~State() = default;

	virtual void OnStateEnter(const Event& e) = 0;
	virtual void OnStateExit(const Event& newEvent) = 0;


	virtual void Render(::Tmpl8::Surface* surface) const override = 0;
	void Tick(float DeltaTime) override {};
};