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

void BoxCollider::UpdateSize(const glm::vec3& newScale)
{
	static PxBoxGeometry box;
	if(_physxShape && _physxShape->getBoxGeometry(box))
	{
		box.halfExtents = GLMVec3ToPxVec3(glm::vec3(_hSize.x * newScale.x, _hSize.y * newScale.y, _hSize.z * newScale.z));
		_physxShape->setGeometry(box);
	}
}