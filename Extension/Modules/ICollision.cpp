#include "../precomp.h"
#include "../Interfaces/ICollision.h"

#include "CollisionModule.h"
#include "../GameObject.h"
#include "../Level.h"
#include "../../RType/Levels/RTypeLevel.h"

Tmpl8::vec2 ICollider::GetCenter() const {
	return owner->pos;
}

bool ICollider::DoesCollide(ICollider* other)
{
	vec2 cp = other->GetClosestPoint(this);
	return DoesCollide(cp);
}

void ICollider::OnCollide(ICollider* other) {
	isColliding = true;
	if (owner != nullptr)
		owner->OnCollide(this, other);
}

Tmpl8::vec4 ICollider::GetLocalBounds() const {
	return vec4(-size->x, -size->y, size->x, size->y) * 0.5f;
}

Tmpl8::vec4 ICollider::GetBounds() const {
	return GetLocalBounds() + vec4(owner->pos.x, owner->pos.y, owner->pos.x, owner->pos.y);
}

Tmpl8::vec4 ICollider::GetScreenSpaceBounds() const {
	return owner->GetLevel()->ToScreenSpace(GetBounds());
}

BoxCollider::BoxCollider(GameObject* Owner, ECollisionMask Mask)
	: ICollider(Owner, Mask) {
	size = &owner->size;
}

bool BoxCollider::DoesCollide(const vec2& other)
{
	const Tmpl8::vec4 aBounds = GetBounds();
	const bool left = aBounds.x <= other.x;
	const bool right = aBounds.z >= other.x;
	const bool up = aBounds.y <= other.y;
	const bool down = aBounds.w >= other.y;

	return (
		left && //x left
		right && // x Right
		up && // Y UP
		down //Y DOWN
		);
}

vec2 BoxCollider::GetClosestPoint(ICollider* collider)
{
	const vec2 ACenter = GetCenter();
	const vec2 BCenter = collider->GetCenter();
	const vec4 bounds = GetBounds();
	vec2 dis = (BCenter - ACenter);

	float xp = ACenter.x + dis.x;
	float yp = ACenter.y + dis.y;

	vec2 bestP = vec2(CLAMP(xp, bounds.x, bounds.z), CLAMP(yp, bounds.y, bounds.w));

	return bestP;
}


CircleCollider::CircleCollider(GameObject* Owner, ECollisionMask Mask) : ICollider(Owner, Mask) {
	size = &owner->size;
}

bool CircleCollider::DoesCollide(const vec2& other)
{
	const float dis = abs((other - GetCenter()).length());
	return dis <= size->x * 0.5f;
}

vec2 CircleCollider::GetClosestPoint(ICollider* collider)
{
	const vec2 ACenter = GetCenter();

	const vec2 BCenter = collider->GetCenter();
	const float tsize = max(size->x, size->y);
	const vec2 disvec = (ACenter - BCenter);
	const float l = disvec.length();

	float dis = min(l, tsize);
	vec2 dir = disvec.normalized() * vec2(dis * 0.5f);

	return ACenter - dir;
}

TileCollider::TileCollider(ECollisionMask Mask, RTypeLevel* Rlvl) : ICollider(nullptr, Mask)
{
	rlvl = Rlvl;
	mapLength = &rlvl->mapLength;
	mapHeight = &rlvl->mapHeight;
	tileSize = &rlvl->tileSize;
	tiles = &rlvl->tiles;
	tileDefinitions = &rlvl->tileDefinitions;
}

TileCollider::~TileCollider() {
	rlvl->GetModule<CollisionModule>()->RemoveObject(this);
}

bool TileCollider::DoesCollide(ICollider* other)
{
	//Special type, we need to see if the bounding box collides with a tile (inner elements are expected to not be colliding)
	float a = GetClosestPoint(other).length();
	return a > 0.001f;
}

bool TileCollider::DoesCollide(const vec2& other)
{
	auto* ti = rlvl->GetTileFromWorldPos(other);
	return ti && ti->optionMask > 0;
}

vec2 TileCollider::GetClosestPoint(ICollider* collider)
{
	const vec4 gbb = collider->GetBounds();

	vec2 corners[4] = {
		vec2(gbb.x, gbb.y) ,//TL
		vec2(gbb.z, gbb.y),//TR
		vec2(gbb.z, gbb.w),//BR
		vec2(gbb.x, gbb.w)//BL
	};

	for (int i = 0; i < 4; ++i)
	{
		if (DoesCollide(corners[i]))
			return corners[i];
	}

	return vec2(0);
}

Tmpl8::vec2 TileCollider::GetCenter() const {
	return vec2(0);
}


#ifdef COLLISION_DEBUG
void BoxCollider::Render(Tmpl8::Surface* Surface) const
{
	const vec4 renderBounds = GetScreenSpaceBounds();
	Surface->Box((int)renderBounds.x, (int)renderBounds.y, (int)renderBounds.z, (int)renderBounds.w, 0xff00000);
}

void CircleCollider::Render(Tmpl8::Surface* Surface) const
{
	const float tsize = max(size->x, size->y) * 0.5f;
	const vec2 center = owner->GetLevel()->ToScreenSpace(GetCenter());

	Surface->circle(static_cast<int>(center.x), static_cast<int>(center.y), static_cast<int>(tsize), 1, 0xff00000);
}

void TileCollider::Render(Tmpl8::Surface* Surface) const
{
	const unsigned short wMap = rlvl->mapLength + 1;
	const unsigned short hMap = rlvl->mapHeight;

	//TODO scale tileSize to scrHeight
	const float drawOffset = rlvl->drawOffset;

	const unsigned short wScreenAmount = SCRWIDTH / rlvl->tileSize + 2;

	const short tileOffset = (short)(drawOffset / static_cast<float>(rlvl->tileSize));
	const short TileRemainder = (short)(static_cast<int>(drawOffset) % rlvl->tileSize);

	if (tileOffset + wScreenAmount < 0)
		return;

	for (unsigned short x = 0; x < wScreenAmount && tileOffset + x < wMap; ++x)
	{
		const int xIndex = tileOffset + x;
		if (xIndex < 0)
			continue;

		for (unsigned short y = 0; y < hMap; ++y)
		{
			const int tileIndex = xIndex + wMap * y;

			const unsigned short tileId = rlvl->tiles[tileIndex];

			if (tileId >= rlvl->tileDefinitions.Size())
				continue;

			const TileInfo* tile = rlvl->tileDefinitions[tileId];

			//Calcualte world space coordinates
			const int xp = (x * rlvl->tileSize) - TileRemainder;
			const int yp = y * rlvl->tileSize;

			if (tile->optionMask > 0)
				Surface->Box(xp,
					yp, xp + rlvl->tileSize, yp + rlvl->tileSize, 0xff0000);
		}
	}
}
#endif