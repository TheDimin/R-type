#include "../precomp.h"
#include "CollisionModule.h"
#include "../Interfaces/ICollision.h"
#include "../Level.h"

void CollisionModule::RegisterObject(ICollider* Collider)
{
	collidibles.Add(Collider);
}

void CollisionModule::RemoveObject(ICollider* Collider)
{
	collidibles.Remove(Collider);
}

void CollisionModule::Clear()
{
	collidibles.Clear();
}

#ifdef COLLISION_DEBUG
int collisionCheckCount = 0;
#endif

void CollisionModule::Tick(float DeltaTime)
{
#ifdef COLLISION_DEBUG
	collisions.Clear();
	collisionCheckCount = 0;
	Level* lvlp = *lvl;
#endif

	//TODO do this in a cheaper way
	for (int i = 0; i < collidibles.Size(); ++i)
	{
		collidibles[i]->isColliding = false;
	}

	for (int selfIndex = 0; selfIndex < collidibles.Size(); ++selfIndex)
	{
		ICollider* self = collidibles[selfIndex];
		if (!self)
		{
			printf("A collider is invalid ? \n");
			continue;
		}
		const ECollisionMask selfMask = self->mask;

		for (int otherIndex = selfIndex + 1; otherIndex < collidibles.Size(); ++otherIndex)
		{
			ICollider* other = collidibles[otherIndex];
			const ECollisionMask otherMask = other->mask;

#ifdef COLLISION_DEBUG
			collisionCheckCount++;
#endif

			const int ll = selfMask & otherMask;

			if (selfMask == ALL || otherMask == ALL || (ll != 0))
				if (self->DoesCollide(other))
				{
#ifdef COLLISION_DEBUG
					collisions.Add({ lvlp->ToScreenSpace(other->GetClosestPoint(self)) ,lvlp->ToScreenSpace(self->GetClosestPoint(other)) });
#endif
					self->OnCollide(other);
					other->OnCollide(self);

					//Dirty fix forces tilemap to check ALL collisions instead of being satisfied with the first one
					if (selfIndex != 0)
						break;
				}
		}
	}
}

inline bool inbetween(vec2 a, vec2 b, vec2 t)
{
	return (((a.x > t.x && b.x < t.x) || (a.x < t.x&& b.x>t.x)) &&
		((a.y > t.y && b.y < t.y) || (a.y < t.y&& b.y >t.y))
		);
}

bool CollisionModule::RayCast(vec2 start, vec2 end, HitInfo& hit)
{
	for (int i = 0; i < collidibles.Size(); ++i)
	{
		ICollider* collider = collidibles[i];
		//Quick bounding test
		//Check collision

		const vec4 b = collider->GetBounds();
		//vec2 cp = vec2(CLAMP())
		//if (inbetween(start, end, collider.))
			//return true;
	}
	return false;
}

void CollisionModule::Render(Tmpl8::Surface* Surface) const
{
#ifdef COLLISION_DEBUG
	char t[100];
	sprintf(t, "CollisionChecks: %i  ColliderCount: %i", collisionCheckCount, collidibles.Size());
	Surface->Print(t, 15, 40, 0xff0000);
#endif

#ifdef COLLISION_DEBUG
	for (int i = 0; i < collidibles.Size(); ++i)
	{
		collidibles[i]->Render(Surface);
	}

	for (int i = 0; i < collisions.Size(); ++i)
	{
		CDraw* collider = &collisions[i];
		collider->p.y = CLAMP(collider->p.y, 4, SCRHEIGHT - 4);
		collider->p2.y = CLAMP(collider->p2.y, 4, SCRHEIGHT - 4);

		Surface->Bar((int)collider->p.x - 2, (int)collider->p.y - 2, (int)collider->p.x + 2, (int)collider->p.y + 2, 0x00ff00);

		Surface->Line(collider->p2.x, collider->p2.y, collider->p.x, collider->p.y, 0x00ff00);

	}
#endif

}
