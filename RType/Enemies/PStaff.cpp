#include "../precomp.h"
#include "PStaff.h"

#include "../../Extension/Modules/CollisionModule.h"
#include "../Extension/WeaponTypes.h"
#include "../Levels/RTypeLevel.h"
#include "FSM/EnemyEvents.h"
#include "FSM/States/EntityKilledState.h"

void WalkState::OnStateEnter(const Event& E)
{
	EntityStateBase::OnStateEnter(E);

}

inline bool collidesBelow(ICollider* TileCollider, vec4 otherBounds)
{
	return TileCollider->DoesCollide(vec2(otherBounds.x, otherBounds.w)) ||
		TileCollider->DoesCollide(vec2(otherBounds.z, otherBounds.w));
}

void WalkState::Tick(float DeltaTime)
{
	GameObject* p = GetLevel()->GetPlayer();
	float moveXDir = owner->pos.normalized().dot(((p->pos - owner->pos) * vec2(1, 0)).normalized());


	ICollider* oc = owner->GetCollider();
	//if (abs(owner->GetVelocity().length()) < owner->speed * 100)
	owner->AddVelocity(vec2(owner->speed * moveXDir, 0));
	//Check distance only on x axis
	float l = abs(((p->pos - owner->pos) * vec2(1, 0)).length());
	if (l <= 350)
	{
		owner->GetFSM()->InvokeEvent(StartAttackEvent());
		return;
	}

	//check if we have a collisoin below the collider, Raycast implementation would have been nicer

	TileCollider* mapcl = GetOwner<PStaff>()->mapcl;

	vec4 b = oc->GetBounds();
	if (!collidesBelow(mapcl, b))
	{
		owner->GetFSM()->InvokeEvent(JumpEvent(vec2(-moveXDir, 2), -owner->speed * 5.f));
		return;
	}

	vec2 targetTile = owner->pos + owner->GetVelocity() * 100;

	if (mapcl->DoesCollide(targetTile))
	{
		//Find out how many tiles we have to jump up
		const int maxJumpHeight = 3;

		for (int i = 0; i < maxJumpHeight; ++i)
		{
			targetTile.y += (float)*mapcl->tileSize;
			if (!mapcl->DoesCollide(targetTile))
			{
				owner->GetFSM()->InvokeEvent(JumpEvent{
					vec2((targetTile - owner->pos).length() * moveXDir * 0.5f ,-(*mapcl->tileSize) * 3.5f),
					owner->speed * 10
					});
				break;
			}
		}

		owner->SetVelocity(vec2(0, 0));
	}
}

void JumpState::OnStateEnter(const Event& E)
{
	vec2 v = static_cast<JumpEvent*>(&const_cast<Event&>(E))->velocity;
	vel = v;
	lostGround = false;
}

void JumpState::OnStateExit(const Event& NewEvent)
{
	owner->SetVelocity(vec2(0));
}


void JumpState::Tick(float DeltaTime)
{
	owner->SetVelocity(vel * DeltaTime * 100);

	vel += owner->gravity * DeltaTime * .5f;

	//if (!lostGround && !owner->GetCollider()->IsColliding())
	//	lostGround = true;

	if (vel.y > 0)
		if (collidesBelow(GetOwner<PStaff>()->mapcl, owner->GetCollider()->GetBounds()))
		{	//FinishedJump
			owner->GetFSM()->InvokeEvent(JumpEvent());
		}
}

void ShootUpwardsState::OnStateEnter(const Event& E)
{
	owner->SetVelocity({});
}

void ShootUpwardsState::Tick(float DeltaTime)
{
	GameObject* p = GetLevel()->GetPlayer();

	if (owner->pos.y > p->pos.y)
		owner->GetWeapon(0)->Activate(p->pos);

	//If we are on the right we assume we are scrolling out of view, don't catch up
	if (p->pos.x > owner->pos.x) return;

	float l = ((owner->pos - p->pos) * vec2(1, 0)).length();
	if (l > 350)
	{
		owner->GetFSM()->InvokeEvent(Event());
	}
}

RocketLauncher::RocketLauncher(Entity& Owner)
	: WeaponDefinition(Owner)
{
	projectileSprite.Set(new Sprite(new Surface("assets/white.png"), 1));
	maxCoolDown = .8f;
	minCoolDown = 0.3f;
	projectileSize = { 10,10 };
	bulletSpeed = 250;
}

void RocketLauncher::CalculateNewPos(float& deltaTime, Tmpl8::vec2& pos, Projectile& bullet) const
{
	bullet.velocity += gravity * deltaTime;
	pos += bullet.velocity * deltaTime;
}

void RocketLauncher::OnProjectileSpawned(Projectile& Bullet)const
{
	Bullet.velocity = (vec2(1, gravity.y * 0.04f) * Bullet.dir).normalized() * bulletSpeed;
}

PStaff::PStaff()
{
	SetCollider<BoxCollider>(ENEMY);
	AddWeapon<RocketLauncher>();
	speed = 2.4f;
	pointsOnKill = 50;

	auto* MoveState = new WalkState();
	auto* AttackState = new ShootUpwardsState();
	auto* jumpState = new JumpState();
	auto* KilledState = new EntityKilledState();

	fsm->RegisterTransition(nullptr, KilledState, new GuardTypeCheck<CollideEvent>());
	fsm->RegisterTransition(MoveState, jumpState, new GuardTypeCheck<JumpEvent>());
	fsm->RegisterTransition(jumpState, MoveState, new GuardTypeCheck<JumpEvent>());

	fsm->RegisterTransition(MoveState, AttackState);
	fsm->RegisterTransition(AttackState, MoveState);

	startState = MoveState;
}

void PStaff::OnCollide(ICollider* Self, ICollider* Other)
{
	EntityWFSM::OnCollide(Self, Other);


	if (dynamic_cast<TileCollider*>(Other))
		pos.y = static_cast<int>(pos.y / GetLevel<RTypeLevel>()->tileSize) * GetLevel<RTypeLevel>()->tileSize + size.y * .4f;
	else
		Other->SetColliding(false);
}

void PStaff::Awake()
{
	EntityWFSM::Awake();
	mapcl = GetLevel()->GetModule<CollisionModule>()->Find<TileCollider>();
}

void PStaff::CalculatePhysics(float& DeltaTime)
{
	//We can do this better if we have time

	EntityWFSM::CalculatePhysics(DeltaTime);

	if (!collidesBelow(mapcl, GetCollider()->GetBounds()))
	{
		velocity += gravity * DeltaTime;
	}
}
