#pragma once
enum ECollisionMask
{
	NONE = 0,
	ENEMY = 1,
	PLAYER = 1 << 1,
	ALL = 0x11111111
};