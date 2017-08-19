#pragma once
#include "PhysX/PxPhysicsAPI.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Core/Memory.h"

#if _DEBUG
#pragma comment(lib,"PhysX3DEBUG_x64.lib")
#pragma comment(lib,"PhysX3CharacterKinematicDEBUG_x64.lib")
#pragma comment(lib,"PhysX3CommonDEBUG_x64.lib")
#pragma comment(lib,"PhysX3CookingDEBUG_x64.lib")
#pragma comment(lib,"PhysXProfileSDKDEBUG.lib")
#pragma comment(lib,"PxTaskDEBUG.lib")
#pragma comment(lib,"PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib,"PhysXVisualDebuggerSDKDEBUG.lib")
#else
#pragma comment(lib,"PhysX3_x64.lib")
#pragma comment(lib,"PhysX3CharacterKinematic_x64.lib")
#pragma comment(lib,"PhysX3Common_x64.lib")
#pragma comment(lib,"PhysX3Cooking_x64.lib")
#pragma comment(lib,"PhysXProfileSDK.lib")
#pragma comment(lib,"PxTask.lib")
#pragma comment(lib,"PhysX3Extensions.lib")
#endif

using namespace physx;

struct RaycastHit
{
public:
	glm::vec3 Position;
	glm::vec3 Normal;
	float hitDistance;

	static int Compare(const void* rh1, const void* rh2);

	inline RaycastHit& operator=(const RaycastHit& other)
	{
		Position = other.Position;
		Normal = other.Normal;
		hitDistance = other.hitDistance;

		return *this;
	}

	inline bool operator<(const RaycastHit& other) const
	{
		return hitDistance < other.hitDistance;
	}

	inline bool operator>(const RaycastHit& other) const
	{
		return hitDistance > other.hitDistance;
	}

	inline bool operator==(const RaycastHit& other) const
	{
		return hitDistance == other.hitDistance;
	}
	
	inline bool operator!=(const RaycastHit& other) const
	{
		return hitDistance != other.hitDistance;
	}
};

class PhysicalBody;

struct TriggerInfo
{
	PhysicalBody* first;
	PhysicalBody* second;

	TriggerInfo(PhysicalBody* one, PhysicalBody* two) : first(one), second(two)
	{

	}

	virtual ~TriggerInfo()
	{
		first = nullptr;
		second = nullptr;
	}

	void Report();
};

class PhysicsHelper
{
protected:
	static class Physics* _physics;

public:
	inline static void SetPhysics(class Physics* physics)
	{
		_physics = physics;
	}

	static bool PerformRaycast(const PxVec3& origin, const PxVec3& direction, float maxDistance, RaycastHit& outHit);
	static bool PerformRaycast(const PxVec3& origin, const PxVec3& direction, float maxDistance, std::vector<RaycastHit>& outHits);
};

class Allocator : public PxAllocatorCallback
{
	void* allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		return Memory::GetInstance()->Allocate(size, typeName, filename, line);
	}

	void deallocate(void* ptr)
	{
		Memory::GetInstance()->Deallocate(ptr);
	}
};

class SimulationEventCallback : public PxSimulationEventCallback
{
protected:
	class Physics* _physics;

public:
	//This is called when a breakable constraint breaks.
	virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count);
	//This is called with the actors which have just been woken up
	virtual void onWake(PxActor **actors, PxU32 count);
	//This is called with the actors which have just been put to sleep.
	virtual void onSleep(PxActor **actors, PxU32 count);
	//This is called when certain contact events occur.
	virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs);
	//This is called with the current trigger pair events.
	virtual void onTrigger(PxTriggerPair *pairs, PxU32 count);

	void SetPhysics(Physics* physics)
	{
		_physics = physics;
	}

	virtual ~SimulationEventCallback()
	{
		_physics = nullptr;
	}
};

class Physics
{
	friend class SimulationEventCallback;

protected:
	PxFoundation* _foundation;
	PxPhysics* _physxSDK;
	PxProfileZoneManager* _zoneManager;
	Allocator _allocator;
	PxDefaultErrorCallback _errorCallback;
	PxCpuDispatcher* _cpuDispatcher;
	SimulationEventCallback* _simulationEventCallback;

	PxMaterial* _material;

	PxScene* _physxScene;

	std::vector<TriggerInfo*> _triggerInfos;

#if _DEBUG
	PxVisualDebuggerConnection* _visualDebuggerConnection;
#endif

protected:
	static PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0,
									  PxFilterData filterData0,
									  PxFilterObjectAttributes attributes1,
									  PxFilterData filterData1,
									  PxPairFlags& pairFlags,
									  const void* constantBlock,
									  PxU32 constantBlockSize);

public:
	Physics();
	~Physics();

	bool Initialize(PxTolerancesScale scale = PxTolerancesScale(), bool bTrackAllocations = false);
	void Shutdown();

	void Resolve();
	void Update(float fixedDeltaTime);

	void Reload(PxTolerancesScale scale = PxTolerancesScale());

	inline PxScene* GetScene() const
	{
		return _physxScene;
	}

	inline PxRigidStatic* CreateStaticActor(const PxTransform& transform) const
	{
		return _physxSDK->createRigidStatic(transform);
	}

	inline PxRigidDynamic* CreateDynamicActor(const PxTransform& transform) const
	{
		PxRigidDynamic* actor = _physxSDK->createRigidDynamic(transform);
		return actor;
	}

	inline PxShape* CreateBoxShape(float hSizeX, float hSizeY, float hSizeZ) const
	{
		return _physxSDK->createShape(PxBoxGeometry(hSizeX, hSizeY, hSizeZ), *_material, true);
	}

	inline PxShape* CreateCapsuleShape(float radius, float halfHeight) const
	{
		return _physxSDK->createShape(PxCapsuleGeometry(radius, halfHeight), *_material, true);
	}
};

glm::vec3 PxVec3ToGLMVec3(const PxVec3& vector);
glm::quat PxQuatToGLMQuat(const PxQuat& quaternion);
PxVec3 GLMVec3ToPxVec3(const glm::vec3& vector);
PxQuat GLMQuatToPxQuat(const glm::quat& rotation);