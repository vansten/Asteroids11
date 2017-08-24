#pragma once

#include "Core/Scene.h"

class GameplayScene : public Scene
{
public:
	virtual bool Initialize(class ResourceManager& resourceManager);
};