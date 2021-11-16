#pragma once
#include "../Extension/Level.h"

class CircleCollisionLevel :
	public Level
{
protected:
	void OnLoad() override;
	void Tick(float DeltaTime) override;
	void Render(Tmpl8::Surface* Surface) const override;
	void MouseMove(const int& x, const int& y) override;

	Tmpl8::vec2 aPos = { 0,0 };

	Tmpl8::vec2 bPos = { 250,250 };
	float aSize = 50;
	float bSize = 150;
};