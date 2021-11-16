#pragma once

namespace Tmpl8
{
	class Surface;
}

class IRender
{
public:
	virtual void Render(Tmpl8::Surface* Surface)const = 0;
	bool shouldRender = true;
};