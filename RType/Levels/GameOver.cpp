#include "../precomp.h"
#include "GameOver.h"
#include "../RType/Module/ScoreModule.h"
#include "MainMenu.h"

void GameOver::OnLoad()
{
	backGround.Set(new BackGround());
	Surface* og = gameInstance->GetScreen();
	memcpy(backgroundSurface.GetBuffer(), og->GetBuffer(), SCRWIDTH * SCRHEIGHT * sizeof(Pixel));

	nameBuffer[MAX_PNAME] = 0;

	sm = GetModule<ScoreModule>();
#ifdef _DEBUG
	//In debug we can skip mainmenu, score is not garunteed to be loaded,in release this is not the case
	sm->LoadTopScore();
#endif

	gameState = sm->HasHighScore() ? SUBMITHIGHSCORE_STATE : CONTINUE_STATE;
}

void GameOver::Tick(float DeltaTime)
{
	blinktime -= DeltaTime;
	if (blinktime < 0)
		blinktime = totalBlinkTime;
}

void GameOver::Render(Tmpl8::Surface* Surface) const
{
	//This is slower but we can afford it here
	Surface->Clear(0);
	//	backgroundSurface.CopyTo(Surface, Surface->GetWidth(), Surface->GetHeight());

	if (gameState == SUBMITHIGHSCORE_STATE)
	{
		Surface->PrintCentre("NEW HIGHSCORE", 150, 5, 0xffdf00);

		char buffer[MAX_PNAME + 10]{};
		buffer[MAX_PNAME + 9] = 0;

		char format[13] = "Name: %s";
		if (nameCursor == 0)
		{
			const int t = static_cast<int>(lerp(0, 4, blinktime / (totalBlinkTime)));
			const int dcount = MIN(t, 3);
			for (int i = 0; i < dcount; ++i)
				format[8 + i] = '.';
			format[8 + dcount] = 0;
		}


		sprintf(buffer, format, nameBuffer);

		Surface->PrintCentre(buffer, 220, 4, 0xffffff);

		if (nameCursor > 2)
			Surface->PrintCentre("press enter to submit", 270, 2, 0xffffff);
	}


	if (sm->GetLives() > 0)
		Surface->PrintCentre("YOU WON", 50, 10, 0xff0000);
	else
		Surface->PrintCentre("GAME OVER", 50, 10, 0xff0000);
	Surface->PrintCentre("PRESS SPACE TO QUIT", SCRHEIGHT - 70, 3, 0xff0000);
}

void GameOver::KeyDown(const int& Key)
{
	if (gameState == SUBMITHIGHSCORE_STATE)
	{
		if (nameCursor < MAX_PNAME && Key >= 4 && Key <= 29)
		{
			char c = (97 - 4) + Key;
			nameBuffer[nameCursor] = c;
			nameCursor++;
		}
		if (Key == 40 && nameCursor > 2)
		{
			sm->SaveScore(nameBuffer);
			gameInstance->LoadLevel(new MainMenu());
		}
		if (Key == 42 && nameCursor > 0)
		{
			nameCursor--;
			nameBuffer[nameCursor] = 0;
		}
	}

	if (Key == 44)
		gameInstance->LoadLevel(new MainMenu());
}
