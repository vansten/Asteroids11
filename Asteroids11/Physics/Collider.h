#pragma once

#include "Physics.h"

class Actor;

class Collider
{
protected:
	Actor* _owner;
	PxShape* _physxShape;
	bool _bIsTrigger;

public:
	Collider(Actor* owner, bool isTrigger = false);
	virtual ~Collider();

	inline PxShape* GetShape() const
	{
		return _physxShape;
	}

	inline bool GetIsTrigger() const
	{
		return _bIsTrigger;
	}

	inline Actor* GetOwner() const
	{
		return _owner;
	}
};

