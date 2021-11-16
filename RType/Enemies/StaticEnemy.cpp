#include "../precomp.h"
#include "StaticEnemy.h"

#include "../../Extension/Level.h"
#include "../Extension/WeaponTypes.h"
#include "..\PlayerShip.h"

StaticEnemy::StaticEnemy()
{
	SetCollider<BoxCollider>(ENEMY);
	AddWeapon<RotatingBurstWeapon>();
	size = { 49,49 };
	pointsOnKill = 50;
}

void StaticEnemy::Tick(float DeltaTime)
{
	Entity::Tick(DeltaTime);
	GetWeapon(0)->Activate(GetLevel()->GetPlayer()->pos);
}

void StaticEnemy::OnCollide(ICollider* Self, ICollider* Other)
{
	Projectile* pr = dynamic_cast<Projectile*>(Other->GetOwner());
	if (pr && dynamic_cast<PlayerShip*>(pr->bulletType->GetOwner()))
		OnKilled();
}
