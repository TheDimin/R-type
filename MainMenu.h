#pragma once
#include "Extension/Level.h"


class MainMenu :
	public Level
{
protected:
	void OnLoad() override;
	void KeyDown(int Key) override;
	void KeyUp(int Key) override;
};

