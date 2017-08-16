#pragma once

#include "Collider.h"

class CapsuleCollider :	public Collider
{
protected:
	float _hHeight;
	float _radius;

public:
	CapsuleCollider(Actor* owner, bool isTrigger, float Height, float Radius);
	virtual~CapsuleCollider();
};

