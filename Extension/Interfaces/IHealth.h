#pragma once
#include <cstdlib>

class IHealth
{
public:
	float GetHealth() const { return health; }
	void AddHealth(float amount) { health += abs(amount); }
	void RemoveHealth(float amount) { health -= abs(amount); }
protected:
	float health = 0;
};
