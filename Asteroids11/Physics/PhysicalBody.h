#pragma once

#include "Physics.h"

class Actor;

class PhysicalBody
{
protected:
	Actor* _owner;
	PxRigidDynamic* _physxActor;

public:
	PhysicalBody(Actor* owner);
	virtual ~PhysicalBody();

	void PreSimulation();
	void PostSimulation();

	void OnTrigger(PhysicalBody* other);
	void OnContact(PhysicalBody* other);

	void AddForce(const glm::vec3& force);

	inline void SetKinematic(bool kinematic)
	{
		if(_physxActor)
		{
			_physxActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematic);
		}
	}

	inline void SetUseGravity(bool useGravity)
	{
		if(_physxActor)
		{
			_physxActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !useGravity);
		}
	}

	inline Actor* GetOwner() const
	{
		return _owner;
	}
};

