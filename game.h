#pragma once
#include "Extension/Helpers/DVector.h"
#include "Extension/Helpers/DUniquePtr.h"

class IModule;
class IRender;
class ITick;
class Level;

namespace Tmpl8 {

	class Game
	{
	public:
		Game();
		~Game();
	public:
		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick(float deltaTime);
		void MouseUp(int button);
		void MouseDown(int button);
		void MouseMove(int x, int y);
		void KeyUp(int key);
		void KeyDown(int key);

		Surface* GetScreen()const { return screen; }

		template<typename ModuleType>
		ModuleType* GetModule();

		template<typename ModuleT>
		ModuleT* RegisterModule();

	public:
		void LoadLevel(Level* NewLevel);
	private:
		void LoadLevelInternal();
		Level* activeLevel = nullptr;
		Level* levelToLoad = nullptr;

	private:
		Surface* screen = nullptr;

		DVector<DUniquePtr<IModule>> modules = DVector<DUniquePtr<IModule>>();
		DVector<IRender*> renderModules = DVector<IRender*>();
		DVector<ITick*> tickModules = DVector<ITick*>();

	};

	template <typename ModuleType>
	ModuleType* Game::GetModule()
	{
		for (int i = 0; i < modules.Size(); ++i)
		{
			if (ModuleType* casted = dynamic_cast<ModuleType*>(modules[i]))
				return casted;
		}

		assert(false && "Failed to find module ");
		return nullptr;
	}

	template <typename ModuleT>
	ModuleT* Game::RegisterModule()
	{
		ModuleT* moduleInstance = new ModuleT();
		//TODO prevent multiple modules from being registerd
		moduleInstance->lvl = &activeLevel;

		modules.Add(moduleInstance);
		if (IRender* renderModule = dynamic_cast<IRender*>(moduleInstance))
			renderModules.Add(renderModule);
		if (ITick* tickModule = dynamic_cast <ITick*>(moduleInstance))
			tickModules.Add(tickModule);

		moduleInstance->OnLoad();

		return moduleInstance;
	}

}; // namespace Tmpl8

/// <summary>
/// Checks if this screenspace coordinate is inside the viewport
/// </summary>
/// <param name="pos"> Coordinate in screenSpace</param>
/// <returns></returns>
bool IsPosInScreen(Tmpl8::vec2 pos);

bool IsXInScreen(float x);