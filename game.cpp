#include "precomp.h" // include this first in every .cpp file
#include "game.h"
#include <iostream>
#include "Extension/Modules/CollisionModule.h"
#include "Extension/Modules/Bullet/BulletModule.h"
#include "RType/Levels/MainMenu.h"
#include "RType/Levels/RTypeLevel.h"
#include "RType/Module/EnemyModule.h"
#include "RType/Module/RespawnModule.h"
#include "RType/Module/ScoreModule.h"

namespace Tmpl8
{
	Game::Game()
	{
	}

	Game::~Game()
	{
		delete activeLevel;
	}

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		RegisterModule<CollisionModule>();
		RegisterModule<EnemyModule>();
		RegisterModule<BulletModule>();
		RegisterModule<RespawnModule>();
		RegisterModule<ScoreModule>();

#ifdef SKIP_MAIN_MENU
		LoadLevel(new RTypeLevel(1));
#else
		LoadLevel(new MainMenu());
#endif
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		//Level loading can only happen at the start of the frame.
		if (levelToLoad != nullptr)
			LoadLevelInternal();
		if (activeLevel == nullptr)return;

#ifdef PERF_DEBUG
		timer perfTimer = timer();
#endif


		for (int i = 0; i < tickModules.Size(); ++i)
		{
			tickModules[i]->Tick(deltaTime);
		}
		activeLevel->Tick(deltaTime);

#ifdef PERF_DEBUG
		float tickTime = perfTimer.elapsed();
		perfTimer.reset();
#endif

		//Render Scene
		activeLevel->Render(screen);

		//Give Modules option to render
		for (int i = 0; i < renderModules.Size(); ++i)
		{
			renderModules[i]->Render(screen);
		}

#ifdef PERF_DEBUG
		char ms[25];
		ms[24] = 0;
		sprintf(ms, "Tick %f MS\n\0", tickTime);
		screen->Print(ms, 15, 10, 0xff0000);

		sprintf(ms, "Render %f MS\n\0", perfTimer.elapsed());
		screen->Print(ms, 15, 25, 0xff0000);
#endif
	}

	void Game::MouseUp(int button)
	{
		activeLevel->MouseUp(button);
	}

	void Game::MouseDown(int button)
	{
		activeLevel->MouseDown(button);
	}

	void Game::MouseMove(int x, int y)
	{
		activeLevel->MouseMove(x, y);
	}

	void Game::KeyUp(int key)
	{
		activeLevel->KeyUp(key);
	}

	void Game::KeyDown(int key)
	{
		activeLevel->KeyDown(key);
	}

	void Game::LoadLevel(Level* newLevel)
	{
		//If we already qued a lvl to load, kill it
		delete levelToLoad;

		levelToLoad = newLevel;
	}

	void Game::LoadLevelInternal()
	{
		for (int i = 0; i < modules.Size(); ++i)
		{
			modules[i]->OnLevelUnload(activeLevel);
		}

		delete activeLevel;

		activeLevel = levelToLoad;
		levelToLoad = nullptr;

		activeLevel->gameInstance = this;
		activeLevel->OnLoad();
	}
	};

bool IsPosInScreen(vec2 pos)
{
	return (pos.x > 0 && pos.x < SCRWIDTH&& pos.y > 0 && pos.y < SCRHEIGHT);
}

bool IsXInScreen(float x)
{
	return x > -50 && x < SCRWIDTH + 50;
}