#pragma once

#include "glm/glm.hpp"

class Actor;

class UITransform
{
protected:
	glm::vec2 _screenPosition;
	glm::vec2 _size;
	float _rotation;

	Actor* _owner;

public:
	UITransform(const glm::vec2& screenPosition = glm::vec2(0.0f, 0.0f), const glm::vec2& size = glm::vec2(1.0f, 1.0f), float rotation = 0.0f);
	UITransform(const UITransform& other);

	inline const glm::vec2& GetScreenPosition() const
	{
		return _screenPosition;
	}

	inline const glm::vec2& GetSize() const
	{
		return _size;
	}

	inline float GetRotation() const
	{
		return _rotation;
	}

	inline Actor* GetOwner() const
	{
		return _owner;
	}

	inline void SetScreenPosition(const glm::vec2& newScreenPosition)
	{
		_screenPosition = newScreenPosition;
	}

	inline void SetSize(const glm::vec2& newSize)
	{
		_size = newSize;
	}

	inline void SetRotation(float newRotation)
	{
		_rotation = newRotation;
	}

	inline void SetOwner(Actor* owner)
	{
		_owner = owner;
	}
};