#include "../precomp.h"
#include "PlayerShip.h"
#include "Levels/RTypeLevel.h"
#include "../Extension/WeaponTypes.h"

PlayerShip::PlayerShip()
{
	sprite = new Sprite(new Surface("assets/Player.png"), 5);
	spriteFrame = 2;
	boostSprite.SetFrame(2);
	//WARNING collision is now checked between player and player shot bullets
	SetCollider<BoxCollider>(static_cast<ECollisionMask>(PLAYER | ENEMY));
	pos = Tmpl8::vec2(100, 150);
	size = vec2(80, 40);
	speed = 3.1f;
	//collisionMask = PLAYER;
	AddWeapon<DefaultPlayerWeapon>();
}

PlayerShip::~PlayerShip()
{
	delete sprite;
}

void PlayerShip::OnCollide(ICollider* Self, ICollider* Other)
{
	if (auto b = dynamic_cast<Projectile*>(Other->GetOwner()))
		if (b->instigator == this)
			return;

#ifndef NO_COLLISION
#ifdef NO_CLIP

	if (dynamic_cast<TileCollider*>(Other))
		return;
#endif

	velocity = vec2();

	GetLevel<RTypeLevel>()->OnPlayerKilled();
#endif
}

void PlayerShip::Tick(float DeltaTime)
{
	//TODO casting to rtype level every frame bad idea?
	const float scrollSpeed = GetLevel<RTypeLevel>()->scrollSpeed * DeltaTime;
	pos.x += scrollSpeed;

	Entity::Tick(DeltaTime);


	//THIS is stupid but running out of time
	{
		if (abs(velocity.y) < .5f)
			spriteFrame = 2;
		else
		{
			float factor = abs(velocity.y) / (speed * .5f);
			factor = CLAMP(factor, 0, 1);

			if (velocity.y < 0)
				spriteFrame = velocity.y < -1;
			else
				spriteFrame = (velocity.y < 1) + 2;
		}
	}
	boostSprite.SetFrame((velocity.x < 0.01f) + 1);

	auto input = vec2(
		GetAsyncKeyState('A') ? -1 : (GetAsyncKeyState('D') ? 1 : 0),
		GetAsyncKeyState('W') ? -1 : (GetAsyncKeyState('S') ? 1 : 0)
	);
	if (input.length() > 0)
		AddVelocity(input.normalized() * speed * DeltaTime * 100);

	if (GetAsyncKeyState(VK_SPACE))
	{
		WeaponDefinition* wd = GetWeapon(0);
		wd->Activate(pos + vec2(10, 0));
	}

	const Tmpl8::vec2 playerSize = size;
	const float scaler = 0.45f;
	const float localx = GetScreenSpacePos().x;

	//TODO fix clamping x, not working because of scrolling
	if (float t = playerSize.x * scaler; localx < t)
		pos.x = GetLevel<RTypeLevel>()->ToScreenSpace(t);
	if (float t = SCRWIDTH - playerSize.x * scaler; localx > t)
		pos.x = GetLevel<RTypeLevel>()->ToScreenSpace(t);
	if (float t = playerSize.y * scaler; pos.y < t)
		pos.y = t;
	if (float t = SCRHEIGHT - playerSize.y * scaler; pos.y > t)
		pos.y = t;
}

void PlayerShip::Render(Tmpl8::Surface* Surface) const
{
	Entity::Render(Surface);
	//const Tmpl8::vec2 ab = pos + bounds.w;

	const Tmpl8::vec2 screenPos = GetScreenSpacePos();

	boostSprite.DrawScaled(screenPos - size * vec2(1.4f, .3f), size, Surface);

#ifdef TILE_COORDINATE

	const Tmpl8::vec2Int tileIndex = GetLevel<RTypeLevel>()->WorldToTileIndex(pos);
	char buff[15];
	sprintf(buff, "x: %i y: %i", tileIndex.x, tileIndex.y);
	Surface->Print(buff, screenPos.x - size.x * .5f, screenPos.y, 0xAAAAAA);
#endif
}