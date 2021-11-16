#pragma once
#include "IRender.h"
#include "../Extension/Modules/CollisionMask.h"
#include "../Extension/Helpers/DVector.h"
#include "../Extension/Helpers/DUniquePtr.h"

class RTypeLevel;
class TileInfo;
class BoxCollider;
class ICollider;
class GameObject;

class ICollider
#ifdef COLLISION_DEBUG
	: public IRender
#endif
{
	//Populair class :)
friend class ICollider;
friend class BoxCollider;
friend class CircleCollider;
friend class TileCollider;
friend class CollisionModule;
public:
	ICollider(GameObject* Owner, ECollisionMask Mask)
		: mask(Mask),
		owner(Owner)
	{
	}
	virtual ~ICollider() = default;

	GameObject* GetOwner() const { return owner; }
	bool IsColliding() const { return isColliding; }
	//hacky function
void SetColliding(bool coll) { isColliding = coll; }
	virtual Tmpl8::vec2 GetCenter()const;
	virtual bool DoesCollide(ICollider * other);
	virtual bool DoesCollide(const vec2 & p) = 0;
	virtual vec2 GetClosestPoint(ICollider * collider) = 0;

	 Tmpl8::vec4 GetLocalBounds()const;
	 Tmpl8::vec4 GetBounds()const;
	 Tmpl8::vec4 GetScreenSpaceBounds()const;
public:
#ifdef COLLISION_DEBUG
	void Render(Tmpl8::Surface * Surface) const override = 0;
#endif

	ECollisionMask mask = ENEMY;
protected:
	void OnCollide(ICollider * other);

	GameObject* owner = nullptr;


	Tmpl8::vec2* size = nullptr;
	bool isColliding;
};

class BoxCollider :public ICollider
{
public:
	BoxCollider(GameObject* Owner, ECollisionMask Mask);
	bool DoesCollide(const vec2& other) override;
	vec2 GetClosestPoint(ICollider* collider) override;
public:
#ifdef COLLISION_DEBUG
	void Render(Tmpl8::Surface* Surface) const override;
#endif
};

class CircleCollider : public ICollider
{
	//Only use uniform circles for now
public:
	CircleCollider(GameObject* Owner, ECollisionMask Mask);
	bool DoesCollide(const vec2& other) override;
	vec2 GetClosestPoint(ICollider* collider) override;
public:
#ifdef COLLISION_DEBUG
	void Render(Tmpl8::Surface* Surface) const override;
#endif
};

class TileCollider : public ICollider
{
public:
	TileCollider(ECollisionMask Mask, RTypeLevel* Rlvl);
	~TileCollider() override;
	bool DoesCollide(ICollider* other) override;
	bool DoesCollide(const vec2& other) override;
	vec2 GetClosestPoint(ICollider* collider) override;
	Tmpl8::vec2 GetCenter() const override;
#ifdef COLLISION_DEBUG
	void Render(Tmpl8::Surface* Surface) const override;
#endif

	unsigned short* mapLength = nullptr;
	unsigned short* mapHeight = nullptr;
	const unsigned short* tileSize = nullptr;
	DVector<unsigned short>* tiles = nullptr;
	DVector<DUniquePtr<TileInfo>>* tileDefinitions = nullptr;
	RTypeLevel* rlvl = nullptr;

};