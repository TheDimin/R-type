#include "../precomp.h" 
#include "WeaponDefinition.h"

#include "Projectile.h"
#include "../../../RType/Levels/RTypeLevel.h"
#include "../Extension/Entity.h"
#include "../Extension/Modules/Bullet/BulletModule.h"
#include "../RType/PlayerShip.h"



WeaponDefinition::WeaponDefinition(Entity& Owner) : owner(&Owner)
{
	projectileSize = { 10,10};
	assert(owner->GetCollider());
	target = owner->GetCollider()->mask == ENEMY ? PLAYER : ENEMY;
}

void WeaponDefinition::Init()
{
	currentCoolDown = Rand(maxCoolDown) + minCoolDown;
}

void WeaponDefinition::Activate(const vec2& TargetPos)
{
	if (IsOnCooldown())return;

	currentCoolDown = maxCoolDown;
	OnActivate(TargetPos);
}

void WeaponDefinition::OnActivate(const vec2& TargetPos)
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	SpawnProjectile(GetOrigin(), CalculateDir(TargetPos));
}

void WeaponDefinition::CalculateNewPos(float& DeltaTime, Tmpl8::vec2& Pos, Projectile& Projectile) const
{
	float bs = bulletSpeed;
	if (dynamic_cast<PlayerShip*>(owner))
		bs += owner->GetLevel<RTypeLevel>()->scrollSpeed;

	Pos += Projectile.dir * bs * DeltaTime;
}

void WeaponDefinition::OnCollide(Projectile& Projectile, ICollider& HitCollider)
{
	if (dynamic_cast<TileCollider*>(&HitCollider))
		Projectile.SetIsActive(false);
}

//Making definitions tick is a terrible idea.
//It breaks the design of being a Definition.
//Until we switch to timers (and give those support for certain operators)
//This is the quickest way to get this to work.
void WeaponDefinition::Tick(float DeltaTime)
{
	//TODO get rid of tick, use timer instead
	if (currentCoolDown > 0)
		currentCoolDown -= DeltaTime;
}

Projectile& WeaponDefinition::SpawnProjectile(const vec2& Origin, const vec2& Dir) const
{
	Projectile* Projectile = owner->bulletModuleRef->GetInactiveBullet();
	Projectile->pos = Origin;
	Projectile->dir = Dir.normalized();
	Projectile->bulletType = const_cast<WeaponDefinition*>(this);
	Projectile->SetCollisionMask(target); //CAN we do some clever shifting instead ?
	Projectile->sprite = projectileSprite;
	Projectile->instigator = owner;
	Projectile->size = projectileSize;
	OnProjectileSpawned(*Projectile);
	return *Projectile;
}


vec2 WeaponDefinition::GetOrigin() const
{
	return owner->pos;
}

vec2 WeaponDefinition::CalculateDir(vec2 TargetPos) const
{
	return TargetPos - GetOrigin();
}