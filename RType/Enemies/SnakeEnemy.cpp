#include "../precomp.h"
#include "SnakeEnemy.h"
#include "FSM/EntityFSM.h"
#include "../Extension/Helpers/Spline.h"

SplineMove::SplineMove()
{
	spline.Set(new Spline);
	spline->AddPoint({ vec2(-102.f, 28.f),vec2(186.f, 583.f),vec2(94.f, -63.f) });
	spline->AddPoint({ vec2(-9.f, 152.f),vec2(303.f, 184.f),vec2(-100.f, -118.f) });
	spline->AddPoint({ vec2(-356.f, -4.f),vec2(493.f, 5.f),vec2(-1.f, -2.f) });
	spline->AddPoint({ vec2(-178.f, -192.f),vec2(671.f, 197.f),vec2(-35.f, 104.f) });
	spline->AddPoint({ vec2(162.f, -46.f),vec2(241.f, 406.f),vec2(-137.f, 147.f) });
	spline->AddPoint({ vec2(-7.f, 22.f),vec2(13.f, 556.f),vec2(-1.f, -12.f) });
}

void SplineMove::Tick(float DeltaTime)
{
	if (spline.IsValid() && owner)
		owner->pos = spline->GetPoint(0.5f);
}

void SplineMove::OnStateEnter(const Event& E)
{

}

SnakeEnemy::SnakeEnemy() : EntityWFSM()
{
	speed = 4;
	auto* MoveState = new SplineMove();

	/// <summary>
	///TEMP
	/// </summary>
	fsm->RegisterTransition(nullptr, MoveState);
	//auto* AnyToKilled = new Transition(nullptr, KilledState, new GuardTypeCheck<CollideEvent>());
	//auto* MoveToDive = new Transition(MoveState, DiveState);

	startState = MoveState;
	//fsm->RegisterTransition(AnyToKilled);
}
