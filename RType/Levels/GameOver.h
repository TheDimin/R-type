#pragma once
#include "../Extension/Level.h"
#include "../RType/Module/ScoreModule.h"
class GameOver : public Level
{
	enum STATE
	{
		SUBMITHIGHSCORE_STATE,
		CONTINUE_STATE
	};

protected:
	void OnLoad() override;
	void Tick(float DeltaTime) override;
	void Render(Tmpl8::Surface* Surface) const override;
	void KeyDown(const int& Key) override;


	ScoreModule* sm = nullptr;

	int nameCursor = 0;
	const float totalBlinkTime = 3;
	float blinktime = 0;

	char nameBuffer[MAX_PNAME + 1]{};
	STATE gameState = CONTINUE_STATE;
	Surface backgroundSurface{ SCRWIDTH,SCRHEIGHT };
};