#pragma once
#include "../Extension/Level.h"

class ScoreModule;

class MainMenu : public Level
{
protected:
	void OnLoad() override;
	void Render(Tmpl8::Surface* Surface) const override;
	void KeyDown(const int& Key) override;
	ScoreModule* sm = nullptr;
};
