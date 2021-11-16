#include "../precomp.h"
#include "BulletModule.h"
#include "Projectile.h"
#include "../Extension/Level.h"

Projectile* BulletModule::GetInactiveBullet()
{
	for (int i = 0; i < bullets.Size() - 1; ++i)
	{
		Projectile* bullet = bullets[i];
		if (bullet->GetIsActive())
			continue;


		bullet->SetIsActive(true);
		return bullet;
	}

	//No bullet found create new one


	return CreateBullet();
}

void BulletModule::OnLoad()
{
}

void BulletModule::OnLevelUnload(Level* OldLevel)
{
	//With the size constructor of DVector we set the active elements to current,
	// By calling clear we reset this active element to 0 without resizing the array
	bullets.Clear();
}

void BulletModule::KillAll()
{
	for (int i = 0; i < bullets.Size(); ++i)
	{
		bullets[i]->SetIsActive(false);
	}
}

Projectile* BulletModule::CreateBullet()
{
	Projectile* b = new Projectile();
	const int ID = bullets.Add(b);
	(*lvl)->RegisterObject(b);

#ifdef DEBUG_BULLET_CREATION
	printf("Created new bullet, BucketSize: '%i' \n", bullets.Size());
#endif
	return b;
}
