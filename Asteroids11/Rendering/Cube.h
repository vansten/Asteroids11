#pragma once

#include "Mesh.h"

class Cube : public Mesh
{
protected:
	float _size;

public:
	Cube(float size);
	Cube(const Cube& cube);
	virtual ~Cube();

	virtual void Initialize() override;

	inline float GetSize() const
	{
		return _size;
	}
};

