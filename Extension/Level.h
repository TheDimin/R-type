#pragma once
#include "BackGround.h"
#include "GameObject.h"
#include "../game.h"
#include "Helpers/DString.h"
#include "Helpers/DUniquePtr.h"
#include "Helpers/DVector.h"


class IModule;

namespace Tmpl8 {
	class Game;
}

class BackGround;

class Level : public ITick, public IRender
{
public:
	virtual ~Level();
	template<typename PlayerType>
	PlayerType* GetPlayer() { return static_cast<PlayerType*>(player); }
	GameObject* GetPlayer() { return player; }
	template <typename ModuleT>
	ModuleT* RegisterModule() const;
	template<typename ModuleType>
	ModuleType* GetModule() const
	{
		return gameInstance->GetModule<ModuleType>();
	}
	float GetTimeSinceLoad() const { return timeSinceLevelLoad; }

	template<typename GameObjectType>
	GameObjectType* CreateObject()
	{
		GameObjectType* obj = new GameObjectType();
		RegisterObject(obj);
		return obj;
	}

	float ToScreenSpace(float source);
	vec4 ToScreenSpace(vec4 source);
	vec2 ToScreenSpace(vec2 source);

	/// <summary>
	/// Register object that had to be created outside of CreateObject
	/// </summary>
	/// <returns>index of object</returns>
	int RegisterObject(GameObject* go);

	void RegisterRender(IRender* RenderObj);
	void RemoveRender(IRender* RenderObj);

	void RegisterTicker(ITick* TickObj);
	void RemoveTicker(ITick* TickObj);
protected:
	friend class Tmpl8::Game;
	virtual void OnLoad();

	/*=========================
	 *	   Game.H overloads
	 *=========================
	 */
	
	void Tick(float DeltaTime) override;
	void Render(Tmpl8::Surface* Surface)const override;

	virtual	void KeyDown(const int& Key) {};
	virtual	void KeyUp(const int& Key) {};
	virtual void MouseMove(const int& x, const int& y) {};
	virtual void MouseDown(int button) {};
	virtual void MouseUp(int button) {};
public:
	float drawOffset = 0;

	Tmpl8::Game* gameInstance = nullptr;
public:
	GameObject* player = nullptr;

	DUniquePtr<BackGround> backGround;
protected:
	DString mapName;
	DVector<DUniquePtr<GameObject>> objects = DVector<DUniquePtr<GameObject>>();
	DVector<ITick*> tickable = DVector<ITick*>();
	DVector<IRender*> renderable = DVector<IRender*>();
	float timeSinceLevelLoad = 0;
};

template <typename ModuleT>
ModuleT* Level::RegisterModule() const
{
	assert(gameInstance);
	return gameInstance->RegisterModule<ModuleT>();
}