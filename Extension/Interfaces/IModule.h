#pragma once
class Level;

class IModule
{
public:
	virtual ~IModule() = default;

	/// <summary>
	/// Called when a Level gets Unloaded
	/// </summary>
	virtual void OnLevelUnload(Level* OldLevel) {};
protected:
	friend class Game;

	/// <summary>
	/// Called after Level reference is set
	/// </summary>
	virtual void OnLoad() {};
	Level** lvl;
};