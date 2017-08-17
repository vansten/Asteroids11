#pragma once

#include "Audio/AudioSource.h"
#include "Core/Memory.h"
#include "Core/Transform.h"

#define STRING(T) #T

class Collider;
class Mesh;
class PhysicalBody;
class Graphics;

class Actor
{
protected:
	Transform _transform;
	Mesh* _mesh;
	PhysicalBody* _physicalBody;
	AudioSource* _audioSource;
	std::vector<Collider*> _colliders;

	bool _enabled;

public:
	Actor();
	Actor(const Actor& actor);
	virtual ~Actor();

	void Shutdown();

	virtual void Update(float deltaTime);
	void PreSimulate();
	void PostSimulate();
	void Render(const class Camera& camera, Graphics* graphics);
	
	void SetTransform(const Transform& transform);

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

	inline void AddCollider(Collider* col)
	{
		_colliders.push_back(col);
	}

	inline void SetMesh(Mesh* mesh)
	{
		_mesh = mesh;
	}

	inline void SetEnabled(bool newEnabled)
	{
		_enabled = newEnabled;
	}

	inline const std::vector<Collider*>& GetColliders() const
	{
		return _colliders;
	}

	inline Transform& GetTransform()
	{
		return _transform;
	}

	inline Mesh* GetMesh() const
	{
		return _mesh;
	}

	inline AudioSource* GetAudioSource() const
	{
		return _audioSource;
	}

	inline PhysicalBody* GetPhysicalBody() const
	{
		return _physicalBody;
	}

	inline bool IsEnabled() const
	{
		return _enabled;
	}
};

