#pragma once
#include "../Extension/Helpers/DUniquePtr.h"

class CSVParser;

namespace Tmpl8
{
	class Sprite;
}

constexpr int CollisionMask = 0x1;
constexpr int TriggerMask = 0x01;

class TileInfo
{

public:
	TileInfo() = default;
	~TileInfo() = default;

	TileInfo(const TileInfo& Other);

	void ParseCSV(CSVParser& fp);
	bool IsCollider() const { return (optionMask & CollisionMask) != 0; }
	bool IsTrigger() const { return (optionMask & TriggerMask) != 0; }

public:
	DUniquePtr<SharedSprite> sprite;
	unsigned short index = 0;
	unsigned short frames = 0;
	unsigned int optionMask = 0;

};