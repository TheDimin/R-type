#pragma once
#include "../Extension/Interfaces/ITick.h"
#include "../Extension/Helpers/DVector.h"
#include "../Extension/Modules/CollisionMask.h"

class ICollider;
class Projectile;
class Entity;

namespace Tmpl8 {
	class Sprite;
	class vec2;
}

/// <summary>
/// Weapon
/// </summary>
class WeaponDefinition : ITick
{
	/// <summary>
	/// The Projectile is allowed to access these properties
	/// </summary>
	friend Projectile;
	friend Entity;

public:
	virtual ~WeaponDefinition() = default;

	/// <summary>
	/// Activate weapon
	/// </summary>
	/// <param name="TargetPos"></param>
	void Activate(const vec2& TargetPos);

	/// <summary>
	/// Checks if weapon can currently fire or is still on cooldown
	/// </summary>
	/// <returns></returns>
	bool IsOnCooldown() const { return currentCoolDown > 0; }

	Entity* GetOwner() { return owner; }
	void IncreaseLevel() { level++; };
protected:
	WeaponDefinition(Entity& Owner);

	/// <summary>
	/// Called after Constructor, Object is in a ready to be used
	/// </summary>
	virtual void Init();

	/// <summary>
	/// Create a new Projectile used by this weapon
	/// </summary>
	/// <param name="Origin"></param>
	/// <param name="Dir"></param>
	Projectile& SpawnProjectile(const vec2& Origin, const vec2& Dir) const;
	/// <summary>
	/// Callback when Weapon spawned a projectile
	/// </summary>
	/// <param name="Projectile"></param>
	virtual void OnProjectileSpawned(Projectile& Projectile) const {}

	/// <summary>
	/// Weapon Got Successfully Activated, here you spawn the Projectile pattern
	/// </summary>
	/// <param name="TargetPos"></param>
	virtual void OnActivate(const Tmpl8::vec2& TargetPos);

	/// <summary>
	/// Called by the Projectile to update its pos
	/// </summary>
	/// <param name="DeltaTime"></param>
	/// <param name="Pos"> Projectile current pos, same as Projectile->pos</param>
	/// <param name="Projectile">The Projectile instance we are updating</param>
	virtual void CalculateNewPos(float& DeltaTime, Tmpl8::vec2& Pos, Projectile& Projectile) const;

	/// <summary>
	/// Callback when Projectile collides, Usefull to allow Projectiles to pass trough x enemies ect
	/// </summary>
	/// <param name="Projectile"></param>
	/// <param name="HitCollider"></param>
	virtual void OnCollide(Projectile& Projectile, ICollider& HitCollider);

	void Tick(float DeltaTime) override;

protected:
	//HELPERS

	/// <summary>
	/// Get the origin of the weapon
	/// </summary>
	/// <returns> The origin of weapon</returns>
	vec2 GetOrigin() const;

	/// <summary>
	/// Get the Direction from to Target from weapon Origin
	/// </summary>
	/// <param name="TargetPos"></param>
	/// <returns></returns>
	vec2 CalculateDir(vec2 TargetPos) const;

protected:
	DUniquePtr<Tmpl8::Sprite> projectileSprite = nullptr;
	Tmpl8::vec2 projectileSize = { 5, 5 };

	ECollisionMask target = ALL;
	Entity* owner = nullptr;

	float bulletSpeed;

	//Weapon cooldown info
	float maxCoolDown = 5;
	float minCoolDown = 1;
	float currentCoolDown = 0;

	/// <summary>
	/// Level of this weapon, Allows you to increase power of weapon ect
	/// </summary>
	int level = 1;
};