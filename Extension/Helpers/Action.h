#pragma once

class Action
{
public:
	Action() = default;
	virtual ~Action() = default;
	virtual bool Run() = 0;

	virtual bool operator() ()
	{
		return Run();
	}
};