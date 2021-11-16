#include "../precomp.h"
#include "BugEnemy.h"
#include "../../Extension/Level.h"
#include "../Extension/WeaponTypes.h"
#include "FSM/EntityFSM.h"
#include "../Extension/FSM/Transition.h"
#include "FSM/EnemyEvents.h"
#include "FSM/States/EntityKilledState.h"


void WaveMove::Tick(float DeltaTime)
{
	owner->AddVelocity(vec2(-owner->speed * 0.3f, 0));
	owner->pos.y = startPosY + cos((GetLevel()->GetTimeSinceLoad() + owner->pos.x / 10)) * 50;

	owner->GetWeapon(0)->Activate(GetLevel()->GetPlayer()->pos);

	if (owner->GetScreenSpacePos().x < SCRWIDTH * 0.2f)
		owner->GetFSM()->InvokeEvent(Event());
}

void WaveMove::OnStateEnter(const Event& E)
{
	startPosY = owner->pos.y;
}

void StrafeToPos::Tick(float DeltaTime)
{
	owner->AddVelocity((targetPos - owner->pos).normalized() * owner->speed);

	owner->GetWeapon(0)->Activate(GetLevel()->GetPlayer()->pos);
}

void StrafeToPos::OnStateEnter(const Event& E)
{
	targetPos = GetLevel()->GetPlayer()->pos;
	targetPos.x = -owner->size.x - 10;
}

BugEnemy::BugEnemy() : EntityWFSM()
{
	SetCollider<BoxCollider>(ENEMY);
	AddWeapon<DefaultWeapon>();
	speed = 4;
	pointsOnKill = 50;

	auto* MoveState = new WaveMove();
	auto* DiveState = new StrafeToPos();
	auto* KilledState = new EntityKilledState();

	auto* AnyToKilled = new Transition(nullptr, KilledState, new GuardTypeCheck<CollideEvent>());
	auto* MoveToDive = new Transition(MoveState, DiveState);

	startState = MoveState;
	fsm->RegisterTransition(AnyToKilled);
	fsm->RegisterTransition(MoveToDive);
}