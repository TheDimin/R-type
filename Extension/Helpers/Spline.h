#pragma once
#include "DVector.h"
#include "../Interfaces/ITick.h"
#include "../Interfaces/IRender.h"
#include "../GameObject.h"

namespace Tmpl8
{
	class vec2;
}

class IClick
{
public:
	virtual void OnClick() {};
	virtual void OnRightClick(const Tmpl8::vec2& pos) {};
	virtual void OnHold(const Tmpl8::vec2& pos) {};
	virtual void OnRelease() {};
};



struct SPoint
{
	SPoint() {}
	SPoint(const vec2& TangetsA, const vec2& P, const vec2& TangetsB)
		: tangetsA(TangetsA + P),
		point(P),
		tangetsB(TangetsB + P)
	{

	}

	friend void operator+(SPoint& Lhs, const vec2& Rhs)
	{
		Lhs.tangetsA += Rhs;
		Lhs.tangetsB += Rhs;
		Lhs.point += Rhs;
	}

	vec2 tangetsA;
	vec2 point;
	vec2 tangetsB;
};

class Spline :
	public ITick
#ifdef _DEBUG
	, public IRender
#endif
{
public:
	Spline() = default;
	Spline(GameObject* Owner) : owner(Owner) {};
	virtual ~Spline() = default;

	void AddPoint(const SPoint& point);
	SPoint* GetSPoint(int index) const;
	Tmpl8::vec2 GetPoint(const float& factor);



	//INTERFACES
public:
	void PrintCode();
#ifdef _DEBUG

	void Tick(float DeltaTime) override;
	void Render(Tmpl8::Surface* Surface) const override;
#else
	void Tick(float DeltaTime) override {};
#endif
private:
	DVector<SPoint> points;
	GameObject* owner = nullptr;
};

class SplinePointObject : public GameObject, public IClick
{
public:
	SplinePointObject();
	void OnRightClick(const Tmpl8::vec2& pos) override;
	void OnHold(const Tmpl8::vec2& pos) override;
	void Tick(float DeltaTime) override {};
	void SetOwner(Spline* p, int Index, int PIndex)
	{
		owner = p;
		index = Index;
		pType = PIndex;
		switch (pType)
		{
		case 0:
			pos = owner->GetSPoint(index)->tangetsA;
			break;
		case 1:
			pos = owner->GetSPoint(index)->point;
			break;
		case 2:
			pos = owner->GetSPoint(index)->tangetsB;
		}
	}
private:
	Spline* owner = nullptr;
	int index = -1;
	int pType = -1;
};