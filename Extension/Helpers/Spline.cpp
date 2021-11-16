#include "../precomp.h"
#include "Spline.h"

#include "../Level.h"
#include "../Interfaces/ICollision.h"

SplinePointObject::SplinePointObject() : GameObject()
{
	SetCollider<CircleCollider>(ALL);
	size = { 10,10 };
}

void SplinePointObject::OnRightClick(const Tmpl8::vec2& Pos)
{
	owner->AddPoint({ vec2(-10),Pos,vec2(10) });
}

void SplinePointObject::OnHold(const Tmpl8::vec2& Pos)
{
	pos = Pos;
	switch (pType)
	{
	case 0:
		owner->GetSPoint(index)->tangetsA = pos;
		break;
	case 1:
		owner->GetSPoint(index)->point = pos;
		break;
	case 2:
		owner->GetSPoint(index)->tangetsB = pos;
	}

	//Report to splinepoint
}

void Spline::AddPoint(const SPoint& point)
{
	int i = points.Add(point);

	if (!owner) return;
	points[i].point += owner->pos;
	points[i].tangetsA += owner->pos;
	points[i].tangetsB += owner->pos;

#if _DEBUG
	owner->GetLevel()->CreateObject<SplinePointObject>()->SetOwner(this, i, 0);
	owner->GetLevel()->CreateObject<SplinePointObject>()->SetOwner(this, i, 1);
	owner->GetLevel()->CreateObject<SplinePointObject>()->SetOwner(this, i, 2);
#endif
}

SPoint* Spline::GetSPoint(int index) const
{
	return &points[index];
}

Tmpl8::vec2 Spline::GetPoint(const float& factor)
{
	const SPoint point = points[1];

	vec2 a = point.tangetsA.lerp(point.point, factor);
	vec2 b = point.point.lerp(point.tangetsB, factor);
	return a.lerp(b, factor);
}

void Spline::PrintCode()
{
	for (int i = 0; i < points.Size(); ++i)
	{
		SPoint p = points[i];
		vec2 pv = p.point;
		vec2 ta = p.tangetsA - pv;
		vec2 tb = p.tangetsB - pv;

		printf("spline.AddPoint({ vec2(%ff, %ff),vec2(%ff, %ff),vec2(%ff, %ff) });\n",
			ta.x, ta.y, pv.x, pv.y, tb.x, tb.y);
	}
}

#ifdef _DEBUG
void Spline::Tick(float DeltaTime)
{
}

void Spline::Render(Tmpl8::Surface* Surface) const
{
	for (int i = 0; i < points.Size() - 1; ++i)
	{
		Surface->Line(points[i].tangetsA, points[i].point, 0x00ff00);
		Surface->Line(points[i].point, points[i].tangetsB, 0x00ff00);

		Surface->Line(points[i].tangetsB, points[i + 1].tangetsA, 0x00ffff);
		//Surface->Line(points[0].x, points[0].y, points[1].x, points[1].y, 0x00ff00);
		for (int t = 0; t < 100; ++t)
		{
			const float fac = static_cast<float>(t) / 100.f;
			const vec2 p1 = points[i].tangetsA.lerp(points[i].point, fac);
			const vec2 p2 = points[i].point.lerp(points[i].tangetsB, fac);
			const vec2 p3 = points[i + 1].tangetsA.lerp(points[i + 1].tangetsA, fac);

			const vec2 pa = p1.lerp(p2, fac);
			const vec2 pb = p2.lerp(p3, fac);
			const vec2 p = pa.lerp(pb, fac);

			Surface->Plot(p.x, p.y, 0xff0000);
		}
	}

	Surface->Line(points[points.Size() - 1].tangetsA, points[points.Size() - 1].point, 0x00ff00);
	Surface->Line(points[points.Size() - 1].point, points[points.Size() - 1].tangetsB, 0x00ff00);

}
#endif