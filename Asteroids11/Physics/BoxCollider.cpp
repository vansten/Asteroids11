#include "BoxCollider.h"

#include "Core/Engine.h"

BoxCollider::BoxCollider(Actor* owner, bool isTrigger, const glm::vec3& size) : Collider(owner, isTrigger), _hSize(size * 0.5f)
{
	_physxShape = Engine::GetInstance()->GetPhysics().CreateBoxShape(_hSize.x, _hSize.y, _hSize.z);
	_physxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !GetIsTrigger());
	_physxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, GetIsTrigger());
}

BoxCollider::~BoxCollider()
{

}
