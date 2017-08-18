#include "Ship.h"

#include "Core/Input.h"

Ship::Ship(float speed) : Actor(), _speed(speed), _shoot(false), _xRestriction(7.0f)
{
}

void Ship::ProcessTransform(float deltaTime)
{
	float xAxis = (int)Input::GetKey(GLFW_KEY_A) - (int)Input::GetKey(GLFW_KEY_D);
	glm::vec3 position = _transform.GetPosition();
	position.x = glm::clamp(position.x + _speed * deltaTime * xAxis, -_xRestriction, _xRestriction);
	_transform.SetPosition(position);
}

void Ship::ProcessShooting()
{
	if(!_shoot && Input::GetKey(GLFW_KEY_SPACE))
	{
		_shoot = true;
		//Spawn projectile actor :)
	}

	if(_shoot && !Input::GetKey(GLFW_KEY_SPACE))
	{
		_shoot = false;
	}
}

void Ship::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	ProcessTransform(deltaTime);
	ProcessShooting();
}
