#include "../precomp.h"
#include "SplineLevel.h"
#include "../Extension/Interfaces/ICollision.h"


CursorObject::CursorObject() : GameObject()
{
	SetCollider<BoxCollider>(ALL);
	size = vec2(5, 5);
}

void CursorObject::Tick(float DeltaTime)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	pos.x = static_cast<float>(x);
	pos.y = static_cast<float>(y);

	if (buttonDown)
		selected->OnHold(pos);
}

void CursorObject::OnCollide(ICollider* self, ICollider* other)
{
	if (buttonDown) return;
	//TODO need statefull collision
	selected = dynamic_cast<IClick*>(other->GetOwner());
	if (!selected)
		buttonDown = false;
}

void CursorObject::MouseDown(int button)
{
	if (button == 1 && selected)
	{
		if (!selected) return;
		selected->OnClick();
		buttonDown = true;
	}
	else if (button == 3)
		if (selected)
			selected->OnRightClick(pos);
}

void CursorObject::MouseUp(int button)
{
	if (button == 1) {
		if (selected)
			selected->OnRelease();
		buttonDown = false;
	}
}


SplineLevel::SplineLevel()
{
	backGround.Set(new BackGround());
}

class FObject : public GameObject
{
public:
	void Tick(float DeltaTime) override {};
};

void SplineLevel::OnLoad()
{
	Level::OnLoad();
	cursor = CreateObject<CursorObject>();
	cursor->pos = { 0,0 };
	spline.Set(new Spline(CreateObject<FObject>()));
	spline->AddPoint({ vec2(-102.f, 28.f),vec2(186.f, 583.f),vec2(94.f, -63.f) });
	spline->AddPoint({ vec2(-9.f, 152.f),vec2(303.f, 184.f),vec2(-100.f, -118.f) });
	spline->AddPoint({ vec2(-356.f, -4.f),vec2(493.f, 5.f),vec2(-1.f, -2.f) });
	spline->AddPoint({ vec2(-178.f, -192.f),vec2(671.f, 197.f),vec2(-35.f, 104.f) });
	spline->AddPoint({ vec2(162.f, -46.f),vec2(241.f, 406.f),vec2(-137.f, 147.f) });
	spline->AddPoint({ vec2(-7.f, 22.f),vec2(13.f, 556.f),vec2(-1.f, -12.f) });

}

void SplineLevel::Render(Tmpl8::Surface* Surface) const
{
	Level::Render(Surface);
#if _DEBUG
	spline->Render(Surface);
#endif
}

void SplineLevel::KeyDown(const int& Key)
{
	Level::KeyDown(Key);
	printf("key: %i \n", Key);
}

void SplineLevel::MouseDown(int Button)
{
	if (Button == 2)
		spline->PrintCode();

	cursor->MouseDown(Button);
}

void SplineLevel::MouseUp(int Button)
{
	cursor->MouseUp(Button);
}
