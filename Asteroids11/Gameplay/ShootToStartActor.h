#pragma once

#include "Actor.h"

class ShootToStartActor : public Actor
{
protected:
	bool _canAdvance;

public:
	virtual void Initialize(ResourceManager& resourceManager) override;

	virtual void Update(float deltaTime);
};