#include "../precomp.h"
#include "TileInfo.h"
#include "../Extension/Files/CSVParser.h"

TileInfo::TileInfo(const TileInfo& Other)
{
	optionMask = Other.optionMask;
	index = Other.index;
	sprite.Set(new Tmpl8::SharedSprite(*Other.sprite.Get()));
}

void TileInfo::ParseCSV(CSVParser& fp)
{
	char* spritePath = fp.ReadChar();
	frames = fp.ReadInt();
	sprite.Set(new Tmpl8::SharedSprite(new Tmpl8::Surface(spritePath), frames));
	index = 0;
	delete[] spritePath;

	optionMask = fp.ReadInt();
}
