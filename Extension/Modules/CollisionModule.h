#pragma once
#include "../Interfaces/IModule.h"
#include "../Helpers/DVector.h"
#include "../Interfaces/ITick.h"
#include "../Interfaces/IRender.h"

class ICollider;
class ICollision;
class GameObject;

struct CDraw
{
	vec2 p;
	vec2 p2;
};

struct HitInfo
{

};

class CollisionModule :
	public IModule, public IRender, public ITick
{
public:
	void RegisterObject(ICollider* Collider);
	void RemoveObject(ICollider* Collider);
	void Clear();
	void Render(Tmpl8::Surface* Surface) const override;
	void Tick(float DeltaTime) override;

	bool RayCast(vec2 start, vec2 end, HitInfo& hit);

	template<typename ColliderType>
	ColliderType* GetCollider(int index);

	template<typename ColliderType>
	ColliderType* Find();

	DVector<CDraw> collisions;

private:
	DVector<ICollider*> collidibles{};
};

template <typename ColliderType>
ColliderType* CollisionModule::GetCollider(int index)
{
	return dynamic_cast<ColliderType*>(collidibles[index]);
}

template <typename ColliderType>
ColliderType* CollisionModule::Find()
{
	for (int i = 0; i < collidibles.Size(); ++i)
	{
		if (ColliderType* t = dynamic_cast<ColliderType*>(collidibles[i]))
			return t;
	}
	return nullptr;
}
