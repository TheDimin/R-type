#include "../precomp.h"
#include "JeffEnemy.h"

#include "../../Extension/Modules/Bullet/BulletModule.h"
#include "../Extension/WeaponTypes.h"
#include "../Levels/RTypeLevel.h"
#include "FSM/EnemyEvents.h"
#include "FSM/States/EntityKilledState.h"


WeaponAttackStage::WeaponAttackStage(unsigned short WeaponID)
	: weaponIndex(WeaponID)
{

}

void WeaponAttackStage::OnStateEnter(const Event& E)
{
	playerPos = &GetLevel()->GetPlayer()->pos;
	moveCooldown = 3;
}

void WeaponAttackStage::Tick(float DeltaTime)
{
	owner->GetWeapon(weaponIndex)->Activate(*playerPos);
	moveCooldown -= DeltaTime;

	if (moveCooldown <= 0)
		owner->GetFSM()->InvokeEvent(Event());
}


void MoveToPlayer::Tick(float DeltaTime)
{
	EntityStateBase::Tick(DeltaTime);
	currentDuration += DeltaTime;
	if (currentDuration > maxDuration)
	{
		owner->GetFSM()->InvokeEvent(MoveComplete());
		return;
	}

	owner->pos = ownerStartPos.lerp(targetPos, currentDuration / maxDuration);


}

void MoveToPlayer::OnStateEnter(const Event& E)
{
	targetPos = owner->GetLevel()->GetPlayer()->pos;
	ownerStartPos = owner->pos;
	currentDuration = 0;
	maxDuration = 3;
}
void MoveToAttackPos::OnStateEnter(const Event& E)
{
	targetPos = vec2(owner->GetLevel()->drawOffset + SCRWIDTH * .8f, static_cast<float>(IRand(SCRHEIGHT - static_cast<int>(owner->size.y) - 150)) + owner->size.y * 0.5f);
	ownerStartPos = owner->pos;
	currentDuration = 0;
	maxDuration = 1.5f;
}

class ClearBulletsAction : public EventAction
{
public:
	bool Run(const Event& e) override
	{
		auto* lvlEvent = dynamic_cast<LevelEvent*>(const_cast<Event*>(&e));
		lvlEvent->lvl->GetModule<BulletModule>()->KillAll();
		return true;
	}
};

class SwitchLevelAction : public EventAction
{
public:
	bool Run(const Event& e) override
	{
		//TODO make this delayed ?
		auto* collideEvent = dynamic_cast<CollideEvent*>(const_cast<Event*>(&e));
		RTypeLevel* rlvl = collideEvent->killer->GetLevel < RTypeLevel >();

		rlvl->gameInstance->LoadLevel(new RTypeLevel(rlvl->levelId + 1));
		return true;
	}
};

class IsInStage :public EventAction
{
public:
	IsInStage(int stageID, JeffEnemy* Jeff) : stage(stageID), jeff(Jeff)
	{
	};

	bool Run(const Event&) override
	{
		return jeff->stage == stage;
	};

	int stage;
	JeffEnemy* jeff;
};

class IncrementStage :public EventAction
{
public:
	IncrementStage(JeffEnemy* Jeff) : jeff(Jeff) {}
	bool Run(const Event&) override
	{
		jeff->stage++;
		return true;
	}
	JeffEnemy* jeff;
};

void BossStateProgress::OnStateEnter(const Event& E)
{
	cooldown = 2;
}

void BossStateProgress::Tick(float DeltaTime)
{
	cooldown -= DeltaTime;
	if (cooldown <= 0)
		owner->GetFSM()->InvokeEvent(LevelEvent(owner->GetLevel()));
}

JeffEnemy::JeffEnemy()
{
	SetCollider<BoxCollider>(ENEMY);
	AddWeapon<DefaultWeapon>();
	AddWeapon<RotatingBurstWeapon>(3);

	//TODO make enemy move into screen after we stopped scrolling

	auto* AttackStageOne = new WeaponAttackStage(0);
	auto* AttackStageTwo = new WeaponAttackStage(1);
	auto* moveToPlayerPos = new MoveToPlayer();
	auto* moveToAttackPos = new MoveToAttackPos();
	auto* DeathState = new BossStateProgress();
	auto* KillState = new EntityKilledState();



	fsm->RegisterTransition(moveToPlayerPos, moveToAttackPos, nullptr);

	fsm->RegisterTransition(moveToAttackPos, AttackStageOne, new IsInStage(0, this), new IncrementStage(this));
	fsm->RegisterTransition(moveToAttackPos, AttackStageTwo, new IsInStage(2, this));

	fsm->RegisterTransition(DeathState, moveToAttackPos, new GuardTypeCheck<LevelEvent>(), new ClearBulletsAction());

	fsm->RegisterTransition(AttackStageOne, DeathState, new GuardTypeCheck<CollideEvent>(), new IncrementStage(this));

	fsm->RegisterTransition(AttackStageTwo, KillState, new GuardTypeCheck<CollideEvent>(), new SwitchLevelAction());
	fsm->RegisterTransition(AttackStageTwo, moveToPlayerPos);

	startState = moveToAttackPos;
}

void JeffEnemy::OnEnable()
{
	EntityWFSM::OnEnable();
	GetLevel<RTypeLevel>()->scrollSpeed = 0;
	stage = 0;
}