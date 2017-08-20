#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
protected:
	glm::vec3 _hSize;

public:
	BoxCollider(Actor* owner, bool isTrigger, const glm::vec3& size);
	virtual~BoxCollider();

	virtual void UpdateSize(const glm::vec3& newScale) override;
};

