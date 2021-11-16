#pragma once
#include "../Extension/Level.h"

class CBox : public GameObject
{
public:
	CBox();
	void Tick(float DeltaTime) override { };
};

class CCircle : public GameObject
{
public:
	CCircle();
	void Tick(float DeltaTime) override { };
};

class AABB
	:
	public Level {
public:
	AABB();
protected:
	void OnLoad() override;
public:
	//void Tick(float DeltaTime) override;
	void Render(Tmpl8::Surface* Surface) const override;
protected:
	void MouseMove(const int& x, const int& y) override;
public:
	Tmpl8::vec2 APos = { 200,200 };
	Tmpl8::vec2 ab = { 50,50 };


	vec2 BPos = { 300,250 };
	vec2 bB = { 50,150 };
};