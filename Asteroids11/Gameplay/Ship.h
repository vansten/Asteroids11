#pragma once

#include "Actor.h"

class Ship : public Actor
{
protected:
	float _xRestriction;
	float _speed;
	bool _shoot;

public:
	Ship(float speed);

protected:
	void ProcessTransform(float deltaTime);
	void ProcessShooting();

public:
	virtual void Update(float deltaTime);
};