#pragma once

#include <vector>

#include "Core/Scene.h"

enum class GrowthStrategy
{
	DOUBLE_SIZE,
	INCREMENT_BY_ONE
};

template<class T>
class ActorsPool
{
protected:
	std::vector<T*> _objects;
	GrowthStrategy _growthStrategy;
	Scene* _scene;

protected:
	void Grow()
	{
		if(!_scene)
		{
			return;
		}

		size_t currentSize = _objects.capacity();
		size_t targetSize = currentSize;
		switch(_growthStrategy)
		{
		case GrowthStrategy::DOUBLE_SIZE:
			targetSize *= 2;
			break;
		case GrowthStrategy::INCREMENT_BY_ONE:
			targetSize += 1;
			break;
		}

		for(size_t i = currentSize; i < targetSize; ++i)
		{
			_objects.push_back(_scene->SpawnActor<T>());
		}
	}

public:
	void Initialize(Scene* scene, int initialCount, GrowthStrategy growthStrategy = GrowthStrategy::DOUBLE_SIZE)
	{
		_scene = scene;
		_growthStrategy = growthStrategy;

		if(_scene)
		{
			for(int i = 0; i < initialCount; ++i)
			{
				_objects.push_back(_scene->SpawnActor<T>());
			}
		}
	}

	void Shutdown()
	{
		_scene = nullptr;

		//No need to destruct objects since they're actors and the scene is responsible for destroying them and cleaning memory :)
	}

	void ReturnToPool(T* object)
	{
		_objects.push_back(object);
	}

	T* GetObject()
	{
		T* object = nullptr;
		if(_objects.size() == 0)
		{
			Grow();
		}
		
		if(_objects.size() > 0)
		{
			object = _objects.at(0);
			_objects.erase(_objects.begin());
		}

		return object;
	}
};

