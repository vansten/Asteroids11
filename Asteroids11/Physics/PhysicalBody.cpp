#include "PhysicalBody.h"

#include "Collider.h"
#include "Core/Engine.h"
#include "Gameplay/Actor.h"

PhysicalBody::PhysicalBody(Actor* owner) : _owner(owner), _physxActor(nullptr)
{
	_physxActor = Engine::GetInstance()->GetPhysics().CreateDynamicActor(owner->GetTransform().GetPhysxTransform());
	_physxActor->userData = this;

	auto& it = _owner->GetColliders().begin();
	auto& end = _owner->GetColliders().end();
	for(; it != end; ++it)
	{
		PxShape* shape = (*it)->GetShape();
		_physxActor->attachShape(*shape);
	}

	Engine::GetInstance()->GetPhysics().GetScene()->addActor(*_physxActor);
}

PhysicalBody::~PhysicalBody()
{
	_owner = nullptr;
	_physxActor = nullptr;
}

void PhysicalBody::PreSimulation()
{
	if(_physxActor)
	{
		_physxActor->setGlobalPose(_owner->GetTransform().GetPhysxTransform());
	}
}

void PhysicalBody::PostSimulation()
{
	if(_physxActor)
	{
		_owner->GetTransform().SetPhysxTransform(_physxActor->getGlobalPose());
	}
}

void PhysicalBody::OnTrigger(PhysicalBody* other)
{
	printf("Trigger\n");
}

void PhysicalBody::OnContact(PhysicalBody* other)
{
	printf("Collider\n");
}

void PhysicalBody::AddForce(const glm::vec3& force)
{
	if(_physxActor)
	{
		_physxActor->addForce(GLMVec3ToPxVec3(force));
	}
}
