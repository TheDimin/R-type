#include "../precomp.h"
#include "AABB.h"
#include "../Extension/Interfaces/ICollision.h"

CBox::CBox()
{
	SetCollider<BoxCollider>(ALL);
}

CCircle::CCircle()
{
	SetCollider<CircleCollider>(ALL);
}

AABB::AABB()
{
	backGround.Set(new BackGround);
}

static GameObject* a = nullptr;
static GameObject* b = nullptr;
static GameObject* c = nullptr;

void AABB::OnLoad()
{
	Level::OnLoad();
	a = CreateObject<CBox>();
	b = CreateObject<CCircle>();

	b->size = vec2(100, 100);
	a->size *= 2;
	//c = CreateObject<CCircle>();
	//c->pos = BPos;
	b->pos = BPos;
}

void AABB::Render(Tmpl8::Surface* Surface) const
{
	Level::Render(Surface);
	{
		//Surface->Box(APos.x - ab.x * .5f, APos.y - ab.y * .5f, APos.x + ab.x * .5f, APos.y + ab.y * .5f, 0x0000ff);
		Surface->Plot(APos.x, APos.y, 0x00ff00);
		//Surface->Box(BPos.x - bB.x, BPos.y - bB.y, BPos.x + bB.x, BPos.y + bB.y, 0xff0000);
		Surface->Plot(BPos.x, BPos.y, 0x00ff00);

		/*float disx = (APos.x - BPos.x);
		vec2 dir = vec2((APos.x - BPos.x) * (ab.x / disx), 0);*/

		float disx = min((APos - BPos).length(), ab.x);
		vec2 dir = (APos - BPos).normalized() * vec2(disx);
		Surface->Line(APos.x, APos.y, BPos.x, BPos.y, 0xff00ff);

		//	const float xdis = APos.x - BPos.x;
		//	const float ydis = APos.y - BPos.y;
			//vec2 dir =
			//	vec2((xdis / ab.x) * xdis, APos.y);// min((ydis / ab.y) * ydis, ab.y));

		vec2 close = APos - dir;


		//	Surface->circle(APos.x, APos.y, ab.x, ab.x, ab.x, 2, 0xff00ff);
			//Surface->circle(BPos.x, BPos.y, bB.x, bB.y, 2, 0xff00ff);




			//float closeX = CLAMP(APos.x - disx, 4, SCRWIDTH - 4); // CLAMP(dir.x, -ab.x, ab.x);
			//float closeY = disx; //APos.y - CLAMP(dir.y, -ab.x, ab.x);
			//Surface->Plot(closeX, closeY, 0xff0000);
			//	float closeX = APos.x + CLAMP(dis.x, -ab.x * 0.5f, ab.x * 0.5f);
				//float closeY = APos.y + CLAMP(dis.y, -ab.y * 0.5f, ab.y * 0.5f);

			//const vec2 ACenter = GetCenter();
			//const vec4 bounds = GetLocalBounds();
			//vec2 dis = (collider->GetCenter() - ACenter).normalized() * vec2(bounds.x, bounds.y);
		Surface->Bar(static_cast<int>(close.x - 4), static_cast<int>(close.y - 4),
			static_cast<int>(close.x + 4), static_cast<int>(close.y + 4), 0xff0000);
	}


	{
		return;
		//vec2 cp = a->GetCollider()->GetClosestPoint(b->GetCollider());
		//Surface->Bar(cp.x - 2, cp.y - 2, cp.x + 2, cp.y + 2, 0xff0000);
		vec2 TL = APos - (BPos * .5f);
		vec2 BR = APos + (BPos * .5f);
		Surface->Box(static_cast<int>(TL.x),
			static_cast<int>(TL.y),
			static_cast<int>(BR.x),
			static_cast<int>(BR.y), 0x0000ff);
		Surface->Plot(APos.x, APos.y, 0x00ff00);

		vec2 dis = (BPos - APos);

		float closeX = APos.x + CLAMP(dis.x, -ab.x * 0.5f, ab.x * 0.5f);
		float closeY = APos.y + CLAMP(dis.y, -ab.y * 0.5f, ab.y * 0.5f);

		Surface->Bar(static_cast<int>(closeX - 2), static_cast<int>(closeY - 2),
			static_cast<int>(closeX + 2), static_cast<int>(closeY + 2), 0xff0000);

		Surface->Box(static_cast<int>(BPos.x - bB.x), static_cast<int>(BPos.y - bB.y),
			static_cast<int>(BPos.x + bB.x), static_cast<int>(BPos.y + bB.y), 0xff0000);
		Surface->Plot(BPos.x, BPos.y, 0x00ff00);
	}
}

void AABB::MouseMove(const int& X, const int& Y)
{
	//Level::MouseMove(X, Y);
	int x, y;
	SDL_GetMouseState(&x, &y);
	APos = Tmpl8::vec2(x, y);
	a->pos = Tmpl8::vec2(x, y);
}