#pragma once
#include "../Extension/Interfaces/IModule.h"
#include "../Extension/Helpers/DVector.h"
#include "../Extension/Helpers/ClassType.h"

#define IdType unsigned short

class Level;
class Entity;
class ClassTypeBase;
class GameObject;

class EnemyModule :
	public IModule
{
public:
	EnemyModule() = default;
	~EnemyModule() override;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="ID">The id of enemy to spawn</param>
	/// <returns>A enemy thats currently not in use</returns>
	Entity* SpawnEnemy(IdType ID, const Tmpl8::vec2& SpawnLoc);
	template<typename EnemyType>
	EnemyType* SpawnEnemy(IdType ID, Tmpl8::vec2& SpawnLoc)
	{
		return SpawnEnemy(ID, SpawnLoc);
	}

	void OnLoad() override;
	void OnLevelUnload(Level* oldLevel) override;

	unsigned short GetEnemyCount()const { return enemyTypes.Size(); }

private:
	void CreateTypeMap();

	template<typename T>
	int RegisterType()
	{
		enemyTypes.Add(new ClassType<T>());
		return enemyPool.Add(DVector<Entity*>());
	}

	Entity* CreateEnemy(IdType ID, const vec2& spawnLoc);

	DVector<DVector<Entity*>> enemyPool = DVector<DVector<Entity*>>();
	DVector<DUniquePtr<ClassTypeBase>> enemyTypes = DVector<DUniquePtr<ClassTypeBase>>();
};