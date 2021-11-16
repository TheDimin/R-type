#pragma once
#include "../Extension/Level.h"
#include "../Extension/Helpers/Spline.h"



class CursorObject : public GameObject
{
public:
	CursorObject();
	void Tick(float DeltaTime) override;

	void MouseDown(int button);
	void MouseUp(int button);

protected:
	void OnCollide(ICollider* self, ICollider* other) override;

	bool buttonDown = false;
	IClick* selected = nullptr;
};

class SplineLevel : public Level
{
public:
	SplineLevel();
protected:
	void OnLoad() override;
	void Render(Tmpl8::Surface* Surface) const override;
	void KeyDown(const int& Key) override;

	void MouseDown(int button) override;
	void MouseUp(int button) override;

	CursorObject* cursor;
	DUniquePtr<Spline> spline;
};