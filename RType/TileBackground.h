#pragma once
#include "../Extension/BackGround.h"
#include "../Extension/Helpers/DVector.h"

#define STARPOOLCOUNT 5
struct StarPool
{
	StarPool()
	{

	}

	StarPool(int StarCount, float Size, const vec2& Velocity, float OffscreenAmount)
		: StarCount(StarCount),
		size(Size),
		velocity(Velocity),
		offscreenSize(OffscreenAmount)
	{
		locations.Resize(StarCount, true);
		colors.Resize(StarCount, true);
	}

	DVector<vec2 >locations;
	DVector<Pixel> colors;

	int StarCount = 0;
	float size = -1;
	vec2 velocity;
	float offscreenSize = 100;
	inline void SetParticle(int index);
};

class TileBackground :
	public BackGround
{
public:
	TileBackground();
	void OnLoad() override;
	void Tick(float DeltaTime) override;
	void Render(Tmpl8::Surface* Surface) const override;

private:
	RTypeLevel* rlvl = nullptr;

	//particles

	StarPool starPools[STARPOOLCOUNT];
	Sprite starSprite{ new Surface("assets/Planet.png"), 1 };
};