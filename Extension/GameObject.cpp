#include "../precomp.h"
#include "GameObject.h"
#include "Level.h"
#include "Interfaces/ICollision.h"
#include "Modules/CollisionModule.h"

Tmpl8::Sprite DefaultSprite = Tmpl8::Sprite(new Tmpl8::Surface("assets/white.png"), 1);

GameObject::GameObject()
{
	pos = Tmpl8::vec2(0, 0);
	sprite = &DefaultSprite;
}

GameObject::GameObject(const Tmpl8::vec2 Pos)
{
	pos = Pos;
	sprite = &DefaultSprite;
}

GameObject::~GameObject()
{
	//Do not delete default sprite plz
}

void GameObject::Awake()
{
	if (isActive)
	{
		OnEnable();
	}
}

void GameObject::Render(Tmpl8::Surface* Surface) const
{
	sprite->SetFrame(spriteFrame);
	//Draw objects from center instead of top left corner
	const Tmpl8::vec2 screenPos = GetScreenSpacePos();

	sprite->DrawScaled(static_cast<int>(screenPos.x - size.x / 2),
		static_cast<int>(screenPos.y - size.y / 2), static_cast<int>(size.x), static_cast<int>(size.y), Surface);
}

void GameObject::SetIsActive(bool ActiveState)
{
	if (ActiveState == isActive) return;

	isActive = ActiveState;

	if (isActive)
		OnEnable();
	else
		OnDisable();
}

void GameObject::CullingUpdate(bool CullingState)
{
	if (IsCulled == CullingState)
		return;

	IsCulled = CullingState;
	if (IsCulled && isActive)
		SetIsActive(false);
}

void GameObject::InvalidateCollision()
{
	if (collider.IsValid())
		level->GetModule<CollisionModule>()->RemoveObject(collider);
	collider.Set(nullptr);
}

const Tmpl8::vec2 GameObject::GetScreenSpacePos() const
{
	return pos - Tmpl8::vec2(level->drawOffset, 0);
}

void GameObject::OnEnable()
{
	if (shouldRender)
		level->RegisterRender(this);

	if (collider.IsValid())
		level->GetModule<CollisionModule>()->RegisterObject(collider);

	level->RegisterTicker(this);
}

void GameObject::OnDisable()
{
	if (shouldRender)
		level->RemoveRender(this);

	if (collider.IsValid())
		level->GetModule<CollisionModule>()->RemoveObject(collider);

	level->RemoveTicker(this);
}