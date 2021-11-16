#include "../precomp.h"
#include "TileBackground.h"

#include "../Extension/Level.h"
#include "Levels/RTypeLevel.h"

TileBackground::TileBackground()
{
	starSprite.SetFlags(Sprite::FLARE);
	const float sizeLookup[] = { 4, 8, 12, 100, 350 };

	const vec2 velocityLookup[] = { {1,0} ,{2.5f,0},{4.f,0},{8,0},{15,0} };
	const int amountLookup[] = { 35,30,10 ,1,1 };
	const float offscreenLookup[]{ 100,100,100,1000,2000 };

	int lookupIndex = IRand(3);

	for (int i = 0; i < STARPOOLCOUNT; ++i)
	{
		starPools[i] = StarPool(amountLookup[i], sizeLookup[i], velocityLookup[i], offscreenLookup[i]);
		for (int x = 0; x < starPools[i].StarCount; ++x)
		{
			starPools[i].SetParticle(x);
			starPools[i].locations[x].x = Rand(SCRWIDTH + starPools[i].offscreenSize * 2.f) - starPools[i].offscreenSize;
		}
	}
}


void TileBackground::OnLoad()
{
	rlvl = dynamic_cast<RTypeLevel*>(level);
}

void TileBackground::Tick(float DeltaTime)
{
	for (int i = 0; i < STARPOOLCOUNT; ++i)
		for (int x = 0; x < starPools[i].StarCount; ++x)
		{
			starPools[i].locations[x] -= vec2((rlvl->scrollSpeed * 0.001f)) * starPools[i].velocity;
			if (starPools[i].locations[x].x < -starPools[i].offscreenSize)
				starPools[i].SetParticle(x);
		}
}
void TileBackground::Render(Tmpl8::Surface* Surface) const
{
	Surface->Clear(0x0000);

	const unsigned short wMap = rlvl->mapLength + 1;
	const unsigned short hMap = rlvl->mapHeight;

	//TODO scale tileSize to scrHeight
	const float drawOffset = level->drawOffset;

	const unsigned short wScreenAmount = SCRWIDTH / rlvl->tileSize + 2;

	const short tileOffset = (short)(drawOffset / static_cast<float>(rlvl->tileSize));
	const short TileRemainder = (short)(static_cast<int>(drawOffset) % rlvl->tileSize);

	if (tileOffset + wScreenAmount < 0)
		return;

	for (int i = 0; i < STARPOOLCOUNT; ++i)
		for (int x = 0; x < starPools[i].StarCount; ++x)
		{
			const vec2 loc = starPools[i].locations[x];
			//Make sure we visible before rendering
			if (loc.x > -starPools[i].offscreenSize && loc.x < SCRWIDTH)
				starSprite.DrawScaled(loc, vec2(starPools[i].size), starPools[i].colors[x], Surface);
			//Surface->Bar(loc,
			//	loc + vec2(starPools[i].size),
			//	starPools[i].colors[x]);
		}


	for (unsigned short x = 0; x < wScreenAmount && tileOffset + x < wMap; ++x)
	{
		const int xIndex = tileOffset + x;
		if (xIndex < 0)
			continue;

		for (unsigned short y = 0; y < hMap; ++y)
		{

			const int tileIndex = xIndex + wMap * y;

			const unsigned short tileId = rlvl->tiles[tileIndex];

			if (tileId >= rlvl->tileDefinitions.Size())
				continue;

			const TileInfo* tile = rlvl->tileDefinitions[tileId];
			if (!tile->sprite.IsValid()) continue;
			Tmpl8::Sprite* sprite = tile->sprite.Get();

			//Calcualte world space coordinates
			const int xp = (x * rlvl->tileSize) - TileRemainder;
			const int yp = y * rlvl->tileSize;

			sprite->SetFrame(tile->index);
			sprite->DrawScaled(xp, yp, rlvl->tileSize, rlvl->tileSize, Surface);

#ifdef TILE_COORDINATE
			if (x > 0 && x < wScreenAmount - 2) {
				char buff[20];
				buff[19] = 0;
				sprintf(buff, "x%iy%hu", xIndex, y);
				Surface->Print(buff, xp, yp + 4, 0xff00ff);
			}
#endif
		}
	}
}

#define IRANDSIZE(Container,Type) IRand(sizeof(Container) / sizeof(Type))

void StarPool::SetParticle(int index)
{
	vec2 np = vec2(size + Rand(offscreenSize) + SCRWIDTH, IRand(SCRHEIGHT - 175));


	const Pixel colorLookup[] = { 0x094D9C,0xffff33,0xffff33,0xFFA500,0xFF0000 };

	Pixel nc = colorLookup[IRANDSIZE(colorLookup, Pixel)];
	locations[index] = np;
	colors[index] = nc;
}