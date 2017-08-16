#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "MathHelper.h"
#include "Physics/Physics.h"

class Transform
{
protected:
	union
	{
		struct
		{
			glm::quat _rotation;
			glm::vec3 _position;
		};
		PxTransform _physxTransform;
	};
	glm::vec3 _scale;
	glm::vec3 _forward;
	glm::vec3 _right;
	glm::vec3 _up;

	glm::mat4 _modelMatrix;

protected:
	inline void UpdateModelMatrix()
	{
		_modelMatrix = glm::translate(_position) * glm::toMat4(_rotation) * glm::scale(_scale);
		_forward = glm::vec3(glm::toMat4(_rotation) * glm::vec4(MathHelper::Forward, 0.0f));
		_right = glm::cross(_forward, MathHelper::Up);
		_up = glm::cross(_right, _forward);
		_right = glm::cross(_forward, _up);
	}

public:
	Transform(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat& rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
	Transform(const Transform& transform);
	~Transform();

	inline const glm::mat4& GetModelToWorldMatrix() const
	{
		return _modelMatrix;
	}

	inline const glm::vec3& GetPosition() const
	{
		return _position;
	}

	inline const glm::quat& GetRotation() const
	{
		return _rotation;
	}

	inline const glm::vec3& GetScale() const
	{
		return _scale;
	}

	inline const glm::vec3& GetForward() const
	{
		return _forward;
	}

	inline const glm::vec3& GetRight() const
	{
		return _right;
	}

	inline const glm::vec3& GetUp() const
	{
		return _up;
	}

	inline const PxTransform& GetPhysxTransform() const
	{
		return _physxTransform;
	}

	inline void SetPhysxTransform(const PxTransform& transform)
	{
		_physxTransform = transform;
		UpdateModelMatrix();
	}

	inline void SetPosition(const glm::vec3& position)
	{
		_position = position;
		UpdateModelMatrix();
	}

	inline void SetRotation(const glm::quat& rotation)
	{
		_rotation = rotation;
		UpdateModelMatrix();
	}

	inline void SetRotation(const glm::vec3& rotation)
	{
		_rotation = glm::quat(rotation);
		UpdateModelMatrix();
	}

	inline void SetScale(const glm::vec3& scale)
	{
		_scale = scale;
		UpdateModelMatrix();
	}

	inline void Translate(const glm::vec3& translation)
	{
		_position += translation;
		UpdateModelMatrix();
	}

	inline void Translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		_position.x += x;
		_position.y += y;
		_position.z += z;
		UpdateModelMatrix();
	}

	inline void Rotate(const glm::vec3& axis, float angle)
	{
		_rotation = glm::rotate(_rotation, angle, axis);
		UpdateModelMatrix();
	}

	inline void Rotate(const glm::vec3& angles)
	{
		Rotate(MathHelper::Up, angles.y);
		Rotate(MathHelper::Right, angles.x);
		Rotate(MathHelper::Forward, angles.z);
		UpdateModelMatrix();
	}

	inline void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		Rotate(MathHelper::Up, y);
		Rotate(MathHelper::Right, x);
		Rotate(MathHelper::Forward, z);
		UpdateModelMatrix();
	}

	inline void Scale(const glm::vec3& scale)
	{
		_scale += scale;
		UpdateModelMatrix();
	}

	inline void Scale(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		_scale.x += x;
		_scale.y += y;
		_scale.z += z;
		UpdateModelMatrix();
	}

	void operator=(const Transform& other);
};

