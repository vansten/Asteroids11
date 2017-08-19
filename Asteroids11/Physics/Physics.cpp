#include "Physics.h"

#include <algorithm>

#include "PhysicalBody.h"
#include "Gameplay/Actor.h"

PxFilterFlags Physics::FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	return PxFilterFlag::eDEFAULT;
}

void TriggerInfo::Report()
{
	if(first && second)
	{
		first->OnTrigger(second);
		second->OnTrigger(first);
	}
}

Physics* PhysicsHelper::_physics = nullptr;

bool PhysicsHelper::PerformRaycast(const PxVec3& origin, const PxVec3& direction, float maxDistance, RaycastHit& outHit)
{
	if(!_physics)
	{
		return false;
	}
	PxScene* scene = _physics->GetScene();
	if(!scene)
	{
		return false;
	}

	std::vector<RaycastHit> rHits;
	PxRaycastHit* touches = Memory::GetInstance()->Allocate<PxRaycastHit>(sizeof(PxRaycastHit) * 8, "PxRaycastHit", __FILE__, __LINE__);
	PxRaycastBuffer hits(touches, 8);
	PxQueryFilterData fd;
	fd.flags |= PxQueryFlag::eNO_BLOCK;
	bool status = scene->raycast(origin, direction, maxDistance, hits, PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE | PxHitFlag::eMTD, fd);
	if(status)
	{
		status = false;
		for(PxU32 i = 0; i < hits.nbTouches; ++i)
		{
			PxRaycastHit& touch = hits.touches[i];
			if(!touch.hadInitialOverlap())
			{
				status = true;
				rHits.push_back(RaycastHit());
				rHits[rHits.size() - 1].hitDistance = touch.distance;
				rHits[rHits.size() - 1].Position = PxVec3ToGLMVec3(touch.position);
				rHits[rHits.size() - 1].Normal = PxVec3ToGLMVec3(touch.normal);
			}
		}
		if(status)
		{
			if(rHits.size() > 1)
			{
				std::sort(rHits.begin(), rHits.end());
			}
			outHit = rHits[0];
		}
	}

	Memory::GetInstance()->Deallocate<PxRaycastHit>(touches);

	return status;
}

bool PhysicsHelper::PerformRaycast(const PxVec3& origin, const PxVec3& direction, float maxDistance, std::vector<RaycastHit>& outHits)
{
	if(!_physics)
	{
		return false;
	}
	PxScene* scene = _physics->GetScene();
	if(!scene)
	{
		return false;
	}

	PxRaycastHit* touches = Memory::GetInstance()->Allocate<PxRaycastHit>(sizeof(PxRaycastHit) * 32, "PxRaycastHit", __FILE__, __LINE__);
	PxRaycastBuffer hits(touches, 32);
	PxQueryFilterData fd;
	fd.flags |= PxQueryFlag::eNO_BLOCK;
	bool status = scene->raycast(origin, direction, maxDistance, hits, PxHitFlag::eMESH_MULTIPLE | PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE | PxHitFlag::eMTD, fd);
	if(status)
	{
		status = false;
		for(PxU32 i = 0; i < hits.nbTouches; ++i)
		{
			PxRaycastHit& touch = hits.touches[i];
			if(!touch.hadInitialOverlap())
			{
				status = true;
				outHits.push_back(RaycastHit());
				outHits[outHits.size() - 1].hitDistance = touch.distance;
				outHits[outHits.size() - 1].Position = PxVec3ToGLMVec3(touch.position);
				outHits[outHits.size() - 1].Normal = PxVec3ToGLMVec3(touch.normal);
			}
		}

		if(outHits.size() > 1)
		{
			std::sort(outHits.begin(), outHits.end());
		}
	}

	Memory::GetInstance()->Deallocate<PxRaycastHit>(touches);

	return status;
}

int RaycastHit::Compare(const void* rh1, const void* rh2)
{
	RaycastHit* first = (RaycastHit*)rh1;
	RaycastHit* second = (RaycastHit*)rh2;
	if(first == nullptr)
	{
		if(second == nullptr)
		{
			return 0;
		}
		return 1;
	}
	else
	{
		if(second == nullptr)
		{
			return -1;
		}
	}

	if(first->hitDistance < second->hitDistance)
	{
		return 1;
	}
	else if(first->hitDistance > second->hitDistance)
	{
		return -1;
	}

	return 0;
}

Physics::Physics() : _foundation(nullptr), _physxSDK(nullptr), _material(nullptr), _physxScene(nullptr), _simulationEventCallback(nullptr), _cpuDispatcher(nullptr), _zoneManager(nullptr)
#if _DEBUG
, _visualDebuggerConnection(nullptr)
#endif
{

}

Physics::~Physics()
{

}

bool Physics::Initialize(PxTolerancesScale scale, bool bTrackAllocations)
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);
	if(!_foundation)
	{
		printf("Error creating PxFoundation\n");
		return false;
	}
	
	_zoneManager = &PxProfileZoneManager::createProfileZoneManager(_foundation);
	if(!_zoneManager)
	{
		printf("Error creating PxProfileZoneManager\n");
		return false;
	}
	
	_physxSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, scale, bTrackAllocations, _zoneManager);
	if(!_physxSDK)
	{
		printf("Error creating PxPhysics\n");
		return false;
	}

	_cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	_simulationEventCallback = new SimulationEventCallback();
	_simulationEventCallback->SetPhysics(this);

	PxSceneDesc sceneDesc(scale);
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = _cpuDispatcher;
	sceneDesc.filterShader = Physics::FilterShader;
	sceneDesc.simulationEventCallback = _simulationEventCallback;

	_physxScene = _physxSDK->createScene(sceneDesc);
	if(!_physxScene)
	{
		printf("Error creating PxScene\n");
		return false;
	}
	
	_material = _physxSDK->createMaterial(1.f, 0.5f, 0.5f);

#if _DEBUG

	PxVisualDebuggerConnectionManager* manager = _physxSDK->getPvdConnectionManager();
	if(manager != nullptr)
	{
		PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
		_visualDebuggerConnection = PxVisualDebuggerExt::createConnection(manager, "127.0.0.1", 5425, 100, connectionFlags);
	}

	_physxSDK->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);

#endif

	return true;
}

void Physics::Shutdown()
{
	auto& it = _triggerInfos.begin();
	for(; it != _triggerInfos.end(); ++it)
	{
		Memory::GetInstance()->Deallocate((*it));
	}
	_triggerInfos.clear();

#if _DEBUG
	if(_visualDebuggerConnection)
	{
		_visualDebuggerConnection->release();
	}
#endif

	if(_simulationEventCallback)
	{
		delete _simulationEventCallback;
		_simulationEventCallback = nullptr;
	}

	if(_cpuDispatcher != nullptr)
	{
		delete _cpuDispatcher;
		_cpuDispatcher = nullptr;
	}

	if(_material != nullptr)
	{
		_material->release();
	}
	
	if(_physxScene != nullptr)
	{
		_physxScene->fetchResults();
		_physxScene->release();
	}

	if(_physxSDK != nullptr)
	{
		_physxSDK->release();
	}

	if(_zoneManager != nullptr)
	{
		_zoneManager->release();
	}

	if(_foundation != nullptr)
	{
		_foundation->release();
	}
}

void Physics::Resolve()
{
	auto& it = _triggerInfos.begin();
	for(; it != _triggerInfos.end(); ++it)
	{
		(*it)->Report();
		Memory::GetInstance()->Deallocate((*it));
		(*it) = nullptr;
	}
	_triggerInfos.clear();
}

void Physics::Update(float fixedDeltaTime)
{
	_physxScene->simulate(fixedDeltaTime);
	_physxScene->fetchResults(true);
}

glm::vec3 PxVec3ToGLMVec3(const PxVec3& vector)
{
	return glm::vec3(vector.x, vector.y, vector.z);
}

glm::quat PxQuatToGLMQuat(const PxQuat& quaternion)
{
	return glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
}

PxVec3 GLMVec3ToPxVec3(const glm::vec3& vector)
{
	return PxVec3(vector.x, vector.y, vector.z);
}

PxQuat GLMQuatToPxQuat(const glm::quat& rotation)
{
	return PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);
}

void SimulationEventCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void SimulationEventCallback::onWake(PxActor** actors, PxU32 count)
{
}

void SimulationEventCallback::onSleep(PxActor** actors, PxU32 count)
{
}

void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
}

void SimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	if(!_physics)
	{
		return;
	}

	for(PxU32 i = 0; i < count; ++i)
	{
		PxTriggerPair& pair = pairs[i];
		if((PxU16)(pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND) != 0)
		{
			PxRigidActor* firstActor = pair.triggerActor;
			PxRigidActor* secondActor = pair.otherActor;
			TriggerInfo* ti = NewObject(TriggerInfo, (PhysicalBody*)firstActor->userData, (PhysicalBody*)secondActor->userData);
			_physics->_triggerInfos.push_back(ti);
		}
	}
}
