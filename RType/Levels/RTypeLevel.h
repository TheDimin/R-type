#pragma once
#include "../Extension/Level.h"
#include "../TileInfo.h"

class TileCollider;

class RTypeLevel : public Level
{
public:
	RTypeLevel(byte levelId);
	~RTypeLevel();
	void LoadMap(const char* filepath);

	Tmpl8::vec2Int WorldToTileIndex(const Tmpl8::vec2& pos)const;
	TileInfo* GetTileFromWorldPos(const Tmpl8::vec2& pos)const;

	/// <summary>
	/// Checks if this object is inside the viewport
	/// </summary>
	/// <param name="Pos">Location in world space</param>
	/// <returns></returns>
	bool IsInViewport(vec2 Pos) const;
protected:
	void OnLoad() override;
	void Tick(float DeltaTime) override;
	friend class PlayerShip;
	void OnPlayerKilled();
private:
	void LoadMap();
	void LoadEnemies();
	void SpawnEnemies(unsigned short xID);

public:
	unsigned short lastSpawned = 0;
	float scrollSpeed = 0;
	DVector<unsigned short> enemies = DVector<unsigned short>();

	byte levelId = 0;
	char filePrefix[50] {};

	DUniquePtr<TileCollider> tileCollider;

	unsigned short mapLength = 0;
	unsigned short mapHeight = 0;
	const unsigned short tileSize = 44;
	DVector<unsigned short> tiles = DVector<unsigned short>();
	DVector<DUniquePtr<TileInfo>> tileDefinitions = DVector<DUniquePtr<TileInfo>>();
};
