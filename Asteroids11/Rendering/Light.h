#pragma once

#include "Core/Transform.h"

class Light
{
protected:
	Transform _transform;
	glm::vec4 _color;

public:
	inline Light() : _transform(nullptr) { }
	inline virtual ~Light() { }

	inline Transform& GetTransform()
	{
		return _transform;
	}

	inline const glm::vec4& GetColor() const
	{
		return _color;
	}

	inline void SetColor(const glm::vec4& color)
	{
		_color = color;
	}

	inline void Update()
	{
		_transform.Update();
	}
};