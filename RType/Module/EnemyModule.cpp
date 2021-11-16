#include "../precomp.h"
#include "EnemyModule.h"

#include "RespawnPoint.h"
#include "../Extension/Entity.h"
#include "../Extension/Level.h"
#include "../RType/Enemies/EnemyTypes.h"
class PlayerShip;

EnemyModule::~EnemyModule()
{
}

void EnemyModule::OnLoad()
{
	CreateTypeMap();
}

void EnemyModule::OnLevelUnload(Level* OldLevel)
{
	for (int ID = 0; ID < enemyPool.Size() + 1; ++ID)
	{
		for (int i = 0; i < enemyPool[ID].Size(); ++i)
		{
			enemyPool[ID][i] = nullptr;
		}
		enemyPool[ID].Clear();
	}
}


void EnemyModule::CreateTypeMap()
{
	RegisterType<RedShip>();
	RegisterType<BugEnemy>();
	RegisterType<PStaff>();
	RegisterType<StaticEnemy>();
	RegisterType<RespawnPoint>();
	RegisterType<JeffEnemy>();
	RegisterType<FinishGamePoint>();
}

Entity* EnemyModule::SpawnEnemy(IdType ID, const vec2& SpawnLoc)
{
	for (int i = 0; i < enemyPool[ID].Size(); i++)
	{
		if (enemyPool[ID][i]->GetIsActive())
			continue;
		enemyPool[ID][i]->pos = SpawnLoc;

		return enemyPool[ID][i];
	}

	return CreateEnemy(ID, SpawnLoc);
}

Entity* EnemyModule::CreateEnemy(IdType ID, const vec2& spawnLoc)
{
	//assert(enemyTypes.Size() <= ID);
	Entity* createdEntity = dynamic_cast<Entity*>(enemyTypes[ID]->CreateNew());
	createdEntity->pos = spawnLoc;
	enemyPool[ID].Add(createdEntity);
	(*lvl)->RegisterObject(createdEntity);
#ifdef DEBUG_ENEMYCREATION
	printf("Created enemy with ID: '%i' BucketSize: '%i'\n", (int)ID, enemyPool[ID].Size());
#endif
	return createdEntity;
}