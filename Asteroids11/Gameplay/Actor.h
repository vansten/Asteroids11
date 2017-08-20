#pragma once

#include "Audio/AudioSource.h"
#include "Core/Memory.h"
#include "Core/ResourceManager.h"
#include "Core/Transform.h"
#include "Rendering/MeshRenderer.h"

#define STRING(T) #T

#define DEFAULT_TYPE		1 << 0
#define SHIP_TYPE			1 << 1
#define ASTEROID_TYPE		1 << 2
#define PROJECTILE_TYPE		1 << 3

class Collider;
class Mesh;
class PhysicalBody;
class Graphics;
class Light;
class Scene;
class Camera;

class Actor
{
protected:
	Transform _transform;
	MeshRenderer* _meshRenderer;
	PhysicalBody* _physicalBody;
	AudioSource* _audioSource;
	std::vector<Collider*> _colliders;

	int _type;
	bool _enabled;

public:
	Actor();
	Actor(const Actor& actor);
	virtual ~Actor();

	virtual void Initialize(ResourceManager& resourceManager);
	void Shutdown();

	virtual void Update(float deltaTime);
	void PreSimulate();
	void PostSimulate();
	void Render(const class Camera& camera, Graphics* graphics, Light* ambientLight, Light* directionalLight);
	
	void SetTransform(const Transform& transform);
	void UpdateSize(const glm::vec3& newScale);
	virtual void OnModelMatrixUpdated();

	virtual void OnTrigger(Actor* other);

	//Checks if objects bounds are visible by camera
	bool IsVisibleByCamera() const;

	Scene* GetScene() const;

	PhysicalBody* CreateRigidbody(bool useGravity);

	inline AudioSource* CreateAudioSource(AudioClip* clip)
	{
		if(_audioSource)
		{
			Memory::GetInstance()->Deallocate<AudioSource>(_audioSource);
			_audioSource = nullptr;
		}

		_audioSource = NewObject(AudioSource, clip);
		_audioSource->SetOwner(this);
		return _audioSource;
	}

	inline MeshRenderer* CreateMeshRenderer(Mesh* mesh, Shader* shader)
	{
		if(_meshRenderer)
		{
			Memory::GetInstance()->Deallocate<MeshRenderer>(_meshRenderer);
			_meshRenderer = nullptr;
		}

		_meshRenderer = NewObject(MeshRenderer, shader);
		_meshRenderer->SetMesh(mesh);
		return _meshRenderer;
	}

	inline void AddCollider(Collider* col)
	{
		_colliders.push_back(col);
	}

	inline void SetEnabled(bool newEnabled)
	{
		if(_enabled ^ newEnabled)
		{
			_enabled = newEnabled;
			if(_enabled)
			{
				_transform.Update();
			}
		}
	}

	inline const std::vector<Collider*>& GetColliders() const
	{
		return _colliders;
	}

	inline Transform& GetTransform()
	{
		return _transform;
	}

	inline MeshRenderer* GetMeshRenderer() const
	{
		return _meshRenderer;
	}

	inline AudioSource* GetAudioSource() const
	{
		return _audioSource;
	}

	inline PhysicalBody* GetPhysicalBody() const
	{
		return _physicalBody;
	}

	inline int GetType() const
	{
		return _type;
	}

	inline bool IsEnabled() const
	{
		return _enabled;
	}
};

