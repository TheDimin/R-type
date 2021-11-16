#include "../precomp.h"
#include "Entity.h"
#include "Level.h"
#include "WeaponTypes.h"
#include "../Extension/Modules/Bullet/BulletModule.h"
#include "../RType/Module/ScoreModule.h"

Entity::Entity()
{

}

void Entity::Awake()
{
	GameObject::Awake();

	bulletModuleRef = level->GetModule<BulletModule>();
}

void Entity::Tick(float DeltaTime)
{
	CalculatePhysics(DeltaTime);
	for (int i = 0; i < weapons.Size(); ++i)
	{
		weapons[i]->Tick(DeltaTime);
	}
}

void Entity::CalculatePhysics(float& DeltaTime)
{
	pos += velocity * DeltaTime *100;
	velocity = vec2().lerp(velocity, DeltaTime);
}


void Entity::Render(Tmpl8::Surface* Surface) const
{
	GameObject::Render(Surface);
#ifdef VELOCITY_DEBUG
	const Tmpl8::vec2 screenPos = GetScreenSpacePos();
	Surface->Arrow(screenPos.x, screenPos.y, screenPos.x + velocity.x * 100, screenPos.y + velocity.y * 100, 0x00ff00);
#endif
}

void Entity::AddVelocity(Tmpl8::vec2 Velocity)
{
	velocity += Velocity;
}

void Entity::SetVelocity(Tmpl8::vec2 Velocity)
{
	velocity = Velocity;
}

vec2 Entity::GetVelocity() const
{
	return velocity;
}

void Entity::OnKilled()
{
	GetLevel()->GetModule<ScoreModule>()->AddScore(pointsOnKill);
	SetIsActive(false);
}
