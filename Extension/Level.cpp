#include "../precomp.h"
#include "Level.h"
#include "BackGround.h"
#include "GameObject.h"
#include "Modules/CollisionModule.h"

Level::~Level()
{
	if (CollisionModule* cmod = GetModule<CollisionModule>())
		cmod->Clear();
}

float Level::ToScreenSpace(float source)
{
	return source + drawOffset;
}

vec4 Level::ToScreenSpace(vec4 source)
{
	return source - vec4(drawOffset, 0, drawOffset, 0);
}

vec2 Level::ToScreenSpace(vec2 source)
{
	return source - vec2(drawOffset, 0);
}

int Level::RegisterObject(GameObject* go)
{
	int index = objects.Add(go);
	go->level = this;
	go->Awake();
	return index;
}

void Level::RegisterRender(IRender* RenderObj)
{
	renderable.Add(RenderObj);
}

void Level::RemoveRender(IRender* RenderObj)
{
	renderable.Remove(RenderObj);
}

void Level::RegisterTicker(ITick* TickObj)
{
	tickable.Add(TickObj);
}

void Level::RemoveTicker(ITick* TickObj)
{
	tickable.Remove(TickObj);
}

void Level::OnLoad()
{
	backGround->level = this;
	backGround->OnLoad();
}
void Level::Tick(float DeltaTime)
{
	timeSinceLevelLoad += DeltaTime;

	backGround->Tick(DeltaTime);

	for (int i = 0; i < tickable.Size(); ++i)
	{
		tickable[i]->Tick(DeltaTime);
	}
}

void Level::Render(Tmpl8::Surface* Surface) const
{
	backGround->Render(Surface);

	for (int i = 0; i < renderable.Size(); ++i)
	{
		renderable[i]->Render(Surface);
	}
}