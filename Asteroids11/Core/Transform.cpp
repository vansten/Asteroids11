#include "Transform.h"
#include "Gameplay/Actor.h"

void Transform::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(_position) * glm::toMat4(_rotation) * glm::scale(_scale);
	_forward = glm::vec3(glm::toMat4(_rotation) * glm::vec4(MathHelper::Forward, 0.0f));
	_right = glm::cross(_forward, MathHelper::Up);
	_up = glm::cross(_right, _forward);
	_right = glm::cross(_forward, _up);

	if(_owner)
	{
		_owner->OnModelMatrixUpdated();
	}
}

Transform::Transform(Actor* owner, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) : _owner(owner), _position(position), _rotation(rotation), _scale(scale), _shouldUpdateMatrix(false)
{
	UpdateModelMatrix();
}

Transform::Transform(const Transform& transform) : _position(transform._position), _rotation(transform._rotation), _scale(transform._scale), _shouldUpdateMatrix(false)
{
	UpdateModelMatrix();
}

Transform::~Transform()
{

}

void Transform::SetScale(const glm::vec3& scale)
{
	_scale = scale;
	_shouldUpdateMatrix = true;
	if(_owner)
	{
		_owner->UpdateSize(_scale);
	}
}

void Transform::operator=(const Transform& other)
{
	_position = other._position;
	_rotation = other._rotation;
	_scale = other._scale;
	UpdateModelMatrix();
}
