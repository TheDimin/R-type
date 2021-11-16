#pragma once
#include "../Extension/Entity.h"

class PlayerShip :
	public Entity
{
public:
	PlayerShip();
	~PlayerShip() override;
protected:
	void OnCollide(ICollider* self, ICollider* other) override;
public:
	void Tick(float DeltaTime) override;
	void Render(Tmpl8::Surface* Surface) const override;

private:
	Sprite boostSprite{ new Surface("assets/PlayerBoost.png"),4 };
};
