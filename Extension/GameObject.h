#pragma once
#include "Interfaces/ITick.h"
#include "Interfaces/IRender.h"
#include "Helpers/DUniquePtr.h"
#include "Modules/CollisionMask.h"

class ICollider;
class Level;

namespace Tmpl8 {
	class Sprite;
}


class GameObject : public ITick, public IRender
{
public:
	GameObject();
	GameObject(const Tmpl8::vec2 Pos);
	virtual ~GameObject();
	virtual void Awake();
	void Render(Tmpl8::Surface* Surface) const override;


	template<typename Lev>
	Lev* GetLevel() const { return static_cast<Lev*>(level); }

	Level* GetLevel() const { return level; }

	void SetIsActive(bool ActiveState);
	bool GetIsActive() const { return isActive; }
	const Tmpl8::vec2 GetScreenSpacePos() const;
	template<typename CT>
	void SetCollider(ECollisionMask Mask);
	ICollider* GetCollider()const { return collider; }
protected:
	virtual void OnEnable();
	virtual void OnDisable();

	friend ICollider;
	virtual void OnCollide(ICollider* self, ICollider* other) {};

	//Injected Update by Level
	friend class RTypeLevel;
	void CullingUpdate(bool CullingState);

public:
	Tmpl8::vec2 pos;
	Tmpl8::Sprite* sprite = nullptr;
	int spriteFrame = 0;
	Tmpl8::vec2 size = Tmpl8::vec2(50, 50);
protected:
	friend Level;
	Level* level = nullptr;
	bool isActive = true;
	bool IsCulled = true;
private:
	void InvalidateCollision();
	DUniquePtr<ICollider> collider;
};

template <typename CT>
void GameObject::SetCollider(ECollisionMask Mask)
{
	InvalidateCollision();

	collider.Set(new CT(this, Mask));
}