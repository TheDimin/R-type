#pragma once
#include "../Extension/Interfaces/IModule.h"
#include "../Extension/Helpers/DVector.h"

class Level;
class Projectile;

class BulletModule : public IModule
{
public:
	Projectile* GetInactiveBullet();

	void OnLoad() override;
	void OnLevelUnload(Level* OldLevel) override;
	void KillAll();

private:
	Projectile* CreateBullet();
	DVector<Projectile*> bullets = DVector<Projectile*>(50);
};