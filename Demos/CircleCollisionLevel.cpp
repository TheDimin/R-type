#include "../precomp.h"
#include "CircleCollisionLevel.h"

void CircleCollisionLevel::OnLoad()
{
	backGround.Set(new BackGround);
}

void CircleCollisionLevel::Tick(float DeltaTime)
{
	Level::Tick(DeltaTime);
	if (GetAsyncKeyState('Z'))
		aSize -= (25 * DeltaTime);
	if (GetAsyncKeyState('X'))
		aSize += (25 * DeltaTime * 10);
}

void CircleCollisionLevel::Render(Tmpl8::Surface* Surface) const
{
	Level::Render(Surface);

	Surface->Bar(static_cast<int>(bPos.x - 5), static_cast<int>(bPos.y - 5), 50, 50, 0x0000ff);

	const Tmpl8::Pixel cColor = (aPos - bPos).length() < aSize + bSize ? 0xff0000 : 0x00ff00;

	for (float i = 0; i < 80 * 6; ++i)
	{
		{
			for (float t = 0; t < 4; ++t)
			{
				const int x = static_cast<int>(cos(i) * (aSize + t * 0.5f));
				const int y = static_cast<int>(sin(i) * (aSize + t * 0.5f));
				Surface->Plot(x + (int)aPos.x, y + (int)aPos.y, cColor);
			}
		}
		{
			for (float t = 0; t < 4; ++t)
			{
				const int x = static_cast<int>(cos(i) * (bSize + t * 0.5f));
				const int y = static_cast<int>(sin(i) * (bSize + t * 0.5f));
				Surface->Plot(x + (int)bPos.x, y + (int)bPos.y, cColor);
			}
		}
	}
	Surface->Line(bPos.x, bPos.y, aPos.x, aPos.y, 0x0000ff);

	//int distance = (int)(aPos - bPos).length();

	//Surface->Print((char*)distance, aPos.x + distance / 2, aPos.y + distance / 2, 0xff0000);
}

void CircleCollisionLevel::MouseMove(const int& X, const int& Y)
{
	aPos += Tmpl8::vec2((float)X, (float)Y);
}
