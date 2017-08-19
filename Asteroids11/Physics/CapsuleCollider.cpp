#include "CapsuleCollider.h"

#include "Core/Engine.h"

CapsuleCollider::CapsuleCollider(Actor* owner, bool isTrigger, float height, float radius) : Collider(owner, isTrigger), _hHeight(height), _radius(radius)
{
	_physxShape = Engine::GetInstance()->GetPhysics().CreateCapsuleShape(_radius, _hHeight);
	_physxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !GetIsTrigger());
	_physxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, GetIsTrigger());
}

CapsuleCollider::~CapsuleCollider()
{}

void CapsuleCollider::UpdateSize(const glm::vec3& newScale)
{
	static PxCapsuleGeometry capsule;
	if(_physxShape && _physxShape->getCapsuleGeometry(capsule))
	{
		capsule.halfHeight = _hHeight * newScale.y;
		capsule.radius = _radius * glm::min(newScale.x, newScale.z);
		_physxShape->setGeometry(capsule);
	}
}
