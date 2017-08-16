#include "Transform.h"

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) : _position(position), _rotation(rotation), _scale(scale)
{
	UpdateModelMatrix();
}

Transform::Transform(const Transform& transform) : _position(transform._position), _rotation(transform._rotation), _scale(transform._scale)
{
	UpdateModelMatrix();
}

Transform::~Transform()
{

}

void Transform::operator=(const Transform& other)
{
	_position = other._position;
	_rotation = other._rotation;
	_scale = other._scale;
	UpdateModelMatrix();
}
