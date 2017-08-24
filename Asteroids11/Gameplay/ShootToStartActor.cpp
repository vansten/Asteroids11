#include "ShootToStartActor.h"

#include "Core/Engine.h"
#include "Core/Input.h"

void ShootToStartActor::Update(float deltaTime)
{
	if(Input::GetKey(GLFW_KEY_SPACE))
	{
		Engine::GetInstance()->LoadGameplayScene();
	}
}
