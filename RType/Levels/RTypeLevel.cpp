#include "../precomp.h"
#include "RTypeLevel.h"

#include "GameOver.h"
#include "../Extension/Files/CSVParser.h"
#include "../TileBackground.h"
#include "../PlayerShip.h"
#include "../../Extension/Modules/CollisionModule.h"
#include "../../Extension/Modules/Bullet/BulletModule.h"
#include "../Module/RespawnModule.h"
#include "../Module/ScoreModule.h"
#include "../RType/Module/EnemyModule.h"

class ScoreModule;
class RespawnModule;
class PlayerShip;

void RTypeLevel::OnLoad()
{
	tileCollider.Set(new TileCollider(ALL, this));
	GetModule<CollisionModule>()->RegisterObject(tileCollider);
	backGround.Set(new TileBackground());

	player = CreateObject<PlayerShip>();
	player->SetIsActive(false);

	Level::OnLoad();

	LoadMap("");
	LoadEnemies();

#ifndef DISABLE_AUTO_SCROLL
	scrollSpeed = 150;
#endif
}

void RTypeLevel::Tick(float DeltaTime)
{
	Level::Tick(DeltaTime);

	//TODO Loop trough all objects, for culling update, can be limited to x a sec ?
	for (int i = 0; i < objects.Size(); ++i)
	{
		GameObject* obj = objects[i];
		obj->CullingUpdate(!IsInViewport(obj->GetScreenSpacePos()));
	}

	drawOffset += DeltaTime * scrollSpeed;

#if _DEBUG
	if (GetAsyncKeyState('Z'))
		scrollSpeed -= 50 * DeltaTime;
	if (GetAsyncKeyState('X'))
		scrollSpeed += 50 * DeltaTime;
	if (GetAsyncKeyState('C'))
		scrollSpeed = 0;
#endif

	const  int spawnCount = (int)((drawOffset + SCRWIDTH + tileSize * 2) / (float)tileSize);
	if (spawnCount > lastSpawned)
	{
		for (unsigned short i = lastSpawned; i < spawnCount; ++i)
			SpawnEnemies(i);
		lastSpawned = spawnCount;
	}
}

void RTypeLevel::OnPlayerKilled()
{
	ScoreModule* sm = GetModule<ScoreModule>();
	sm->RemoveLife();
	if (sm->GetLives() < 0)
	{
		gameInstance->LoadLevel(new GameOver());
		return;
	}

	player->pos = GetModule<RespawnModule>()->GetRespawnPoint();
	drawOffset = player->pos.x - SCRWIDTH * 0.5f;
	GetModule<BulletModule>()->KillAll();

	lastSpawned = (int)((player->pos.x + SCRWIDTH) / (float)tileSize);

#ifndef DISABLE_AUTO_SCROLL
	scrollSpeed = 150;
#endif
}

RTypeLevel::RTypeLevel(byte LevelId)
{
	levelId = LevelId;
	filePrefix[49] = 0;
	sprintf(filePrefix, "Levels/%i/", levelId);
}

RTypeLevel::~RTypeLevel()
{
}

void RTypeLevel::LoadMap(const char* filepath)
{
	char path[50];
	strcpy(path, filePrefix);
	strncat(path, "Style.csv", strlen("Style.csv"));
	CSVParser StyleParser = CSVParser(path);

	while (!StyleParser.ReachedEndOfData())
	{
		TileInfo* tile = new TileInfo();

		tile->ParseCSV(StyleParser);
		tileDefinitions.Add(tile);

		for (unsigned short i = 1; i < tile->frames; ++i)
		{
			auto* ti = new TileInfo(*tile);
			ti->index = i;
			tileDefinitions.Add(ti);
		}

		StyleParser.NextColumn();
	}
	path[0] = '\0';
	strcpy(path, filePrefix);
	strncat(path, "Map_Map.csv", strlen("Map_Map.csv"));

	CSVParser MapParser = CSVParser(path);
	while (!MapParser.ReachedEndOfData())
	{
		while (!MapParser.ReachedEndOfRow())
		{
			int iValue = MapParser.ReadInt();
			tiles.Add(iValue);
		}

		MapParser.NextColumn();
	}

	mapLength = MapParser.GetRows() - 1;
	mapHeight = MapParser.GetColmns();
}

Tmpl8::vec2Int RTypeLevel::WorldToTileIndex(const Tmpl8::vec2& pos) const
{
	const int vx = static_cast<int>(pos.x / tileSize);
	const int vy = static_cast<int>(pos.y / tileSize);


	return Tmpl8::vec2Int(CLAMP(vx, 0, mapLength), CLAMP(vy, 0, mapHeight));
}

TileInfo* RTypeLevel::GetTileFromWorldPos(const Tmpl8::vec2& pos) const
{
	Tmpl8::vec2Int tilepos = WorldToTileIndex(pos);
	const unsigned short tilePos1D = tilepos.x + tilepos.y * (mapLength + 1);

	if (tilePos1D > tiles.Size())
		return nullptr;

	const unsigned short index = tiles[tilePos1D];

	if (index < 0 || index > tileDefinitions.Size())
		return nullptr;

	return tileDefinitions[index];
}

bool RTypeLevel::IsInViewport(vec2 Pos) const
{
	//We only check if x is in allowed screenspace, we should check y for objects that have upward/downward velocity
	return Pos.x > -50 && Pos.x < SCRWIDTH + 150 && Pos.y > -300 && Pos.y < SCRHEIGHT + 300;
}

void RTypeLevel::LoadEnemies()
{
	char path[50];
	strcpy(path, filePrefix);
	strncat(path, "Map_Enemies.csv", strlen("Map_Enemies.csv"));

	CSVParser enemyParser = CSVParser(path);

	enemies.Resize(mapLength * mapHeight, true);

	//Warning parse x,y inversed, helps mem allignment, we can do this much smarter in a update
	int y = 0;
	while (!enemyParser.ReachedEndOfData()) // y
	{
		int x = 0;
		while (!enemyParser.ReachedEndOfRow()) //x
		{
			if (x * y + y > enemies.Size())
			{
				assert(true);
				return;
			}

			enemies[x * mapHeight + y] = enemyParser.ReadInt();
			x++;
		}
		y++;
		enemyParser.NextColumn();
	}
}

void RTypeLevel::SpawnEnemies(unsigned short xID)
{
	EnemyModule* em = GetModule<EnemyModule>();
	const unsigned short enemyIdCount = em->GetEnemyCount();
	const vec2 offset = vec2(tileSize) * 0.5f;

	for (int y = 0; y < mapHeight; ++y)
	{
		unsigned short ID = enemies[xID * mapHeight + y];

		if (ID > enemyIdCount)
			continue;


		Entity* e = em->SpawnEnemy(ID, Tmpl8::vec2(static_cast<float>(xID * tileSize), static_cast<float>(y * tileSize)) + offset);

		if (e == nullptr) {
			continue;
		}

		e->SetIsActive(true);
	}
}