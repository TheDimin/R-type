#pragma once
#include "../../RType/Levels/RTypeLevel.h"

class GameObject;

class ClassTypeBase
{
public:
	ClassTypeBase() = default;
	virtual ~ClassTypeBase() = default;
	virtual GameObject* CreateNew() = 0;

	/*friend bool operator==(const ClassTypeBase& lhs, const ClassTypeBase& rhs)
	{
		//return typeid(lhs).hash_code() == typeid(rhs).hash_code();
	}

	friend bool operator!=(const ClassTypeBase& lhs, const ClassTypeBase& rhs)
	{
		return !(lhs == rhs);
	}*/
};

template<typename classT>
class ClassType : public ClassTypeBase
{
public:
	GameObject* CreateNew() override
	{
		return new classT();
	}
};