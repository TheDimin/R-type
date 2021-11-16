#pragma once
#include "GameObject.h"

class BackGround : public ITick, public IRender
{
public:
	virtual void OnLoad() {}

	void Tick(float DeltaTime) override {};
	void Render(Tmpl8::Surface* Surface) const override { Surface->Clear(0); };

	Level* level = nullptr;
};

