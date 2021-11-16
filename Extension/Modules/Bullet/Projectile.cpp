#include "../precomp.h"
#include "Projectile.h"
#include "../Extension/Level.h"
#include "../Extension/Modules/CollisionModule.h"
#include "WeaponDefinition.h"

Projectile::Projectile()
{
	SetCollider<CircleCollider>(ENEMY);
}

void Projectile::Awake()
{
	GameObject::Awake();
}

void Projectile::OnCollide(ICollider* self, ICollider* other)
{
	hitCount++;
	bulletType->OnCollide(*this, *other);
}

void Projectile::Tick(float DeltaTime)
{
	bulletType->CalculateNewPos(DeltaTime, pos, *this);
}