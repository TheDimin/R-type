#include "../precomp.h"
#include "RedShip.h"
#include "../Extension/WeaponTypes.h"
#include "../Levels/RTypeLevel.h"

RedShip::RedShip()
{
	SetCollider<BoxCollider>(ENEMY);
	AddWeapon<DefaultWeapon>();
	speed = .5f;
	pointsOnKill = 10;
}

RedShip::~RedShip()
{
#ifdef OBJECT_DESTRUCTOR
	printf("RED DELETED\n");
#endif
}

void RedShip::OnEnable()
{
	Entity::OnEnable();

	xOdd = (int)pos.x / GetLevel<RTypeLevel>()->tileSize % 2;
	startY = pos.y;
	if (xOdd)
		startY += size.y + 50;

	timeSinceStart = level->GetTimeSinceLoad();
	MoveHeight = rand() % 30 + 10;
}

void RedShip::OnCollide(ICollider* self, ICollider* other)
{
	Entity::OnCollide(self, other);
	if (dynamic_cast<Projectile*>(other->GetOwner()))
		OnKilled();
}

void RedShip::Tick(float DeltaTime)
{
	Entity::Tick(DeltaTime);
	//TODO make this a lerpable value
	const float p = (level->GetTimeSinceLoad());/// timeSinceStart*100;
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	/*	const float ep = p < 0.5f
			? (pow(2 * p, 2) * ((c2 + 1) * 2 * p - c2)) / 2
			: (pow(2 * p - 2, 2) * ((c2 + 1) * (p * 2 - 2) + c2) + 2) / 2;
			*/
	const short positive = xOdd ? 1 : -1;

	//pos.y = startY + sin(p) * positive * MoveHeight;

	AddVelocity(vec2(-speed, 0));

	pos.y = startY + sin(p * positive) * 50;

	GetWeapon(0)->Activate(level->GetPlayer()->pos);
}