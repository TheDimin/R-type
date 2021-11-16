#pragma once
#include "GameObject.h"
#include "../Extension/Interfaces/ICollision.h"
#include "Helpers/DVector.h"
#include "Modules/Bullet/WeaponDefinition.h"

class WeaponDefinition;
class BulletModule;

class Entity :
	public GameObject
{
public:
	Entity();
	void Awake() override;
	void Tick(float DeltaTime) override;
	virtual void CalculatePhysics(float& DeltaTime);
	//GameObject* Shoot(const Tmpl8::vec2 origin, const Tmpl8::vec2 dir, BulletTypeInfo& Weapon);

	void Render(Tmpl8::Surface* Surface) const override;
public:
	void AddVelocity(Tmpl8::vec2 velocity);
	void SetVelocity(Tmpl8::vec2 velocity);
	vec2 GetVelocity() const;

	template<typename WeaponType>
	void AddWeapon(int level = 1) {
		WeaponDefinition* wp = new WeaponType(*this);
		wp->level = level;
		wp->Init();
		weapons.Add(wp);
	}
	WeaponDefinition* GetWeapon(const int index) const { return weapons[index]; }

	virtual void OnKilled();

public:
	//GeneralSpeed of player
	float speed = 150;

	const Tmpl8::vec2 gravity{ 0,150 };
protected:
	Tmpl8::vec2 velocity{ 0,0 };
	int pointsOnKill = 0;
private:
	friend class WeaponDefinition;
	BulletModule* bulletModuleRef = nullptr;

	//By default entities do not care about gravity, but single accespoint good
	DVector<DUniquePtr<WeaponDefinition>> weapons;
};
