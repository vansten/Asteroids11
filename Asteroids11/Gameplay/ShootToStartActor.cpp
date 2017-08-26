#include "ShootToStartActor.h"

#include "Core/Engine.h"
#include "Core/Input.h"

void ShootToStartActor::Initialize(ResourceManager& resourceManager)
{
	Actor::Initialize(resourceManager);

	_canAdvance = !Input::GetKey(GLFW_KEY_SPACE);
}

void ShootToStartActor::Update(float deltaTime)
{
	if(!_canAdvance)
	{
		//Wait until space is released (to avoid accidentally loading scene)
		_canAdvance = !Input::GetKey(GLFW_KEY_SPACE);
		return;
	}

	if(Input::GetKey(GLFW_KEY_SPACE))
	{
		Engine::GetInstance()->LoadGameplayScene();
	}
}
