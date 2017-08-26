#include "Scene.h"

#include "Engine.h"
#include "Rendering/Light.h"

void Scene::Shutdown()
{
	auto& uiIT = _uiActors.begin();
	auto& uiEnd = _uiActors.end();
	for(; uiIT != uiEnd; ++uiIT)
	{
		(*uiIT)->Shutdown();
		Memory::GetInstance()->Deallocate<UIActor>(*uiIT);
		(*uiIT) = nullptr;
	}
	_uiActors.clear();

	uiIT = _pendingUIActors.begin();
	uiEnd = _pendingUIActors.end();
	for(; uiIT != _pendingUIActors.end(); ++uiIT)
	{
		(*uiIT)->Shutdown();
		Memory::GetInstance()->Deallocate<UIActor>(*uiIT);
		(*uiIT) = nullptr;
	}
	_pendingUIActors.clear();

	auto& it = _actors.begin();
	auto& end = _actors.end();
	for(; it != end; ++it)
	{
		(*it)->Shutdown();
		Memory::GetInstance()->Deallocate<Actor>(*it);
		(*it) = nullptr;
	}
	_actors.clear();

	it = _pendingActors.begin();
	end = _pendingActors.end();
	for(; it != end; ++it)
	{
		(*it)->Shutdown();
		Memory::GetInstance()->Deallocate<Actor>(*it);
		(*it) = nullptr;
	}
	_pendingActors.clear();
	
	if(_camera)
	{
		_camera->Shutdown();
		Memory::GetInstance()->Deallocate<Camera>(_camera);
		_camera = nullptr;
	}

	if(_ambientLight)
	{
		Memory::GetInstance()->Deallocate<Light>(_ambientLight);
		_ambientLight = nullptr;
	}

	if(_directionalLight)
	{
		Memory::GetInstance()->Deallocate<Light>(_directionalLight);
		_directionalLight = nullptr;
	}
}

void Scene::Update(GLfloat deltaTime)
{
	if(_pendingActors.size() > 0)
	{
		auto& it = _pendingActors.begin();
		auto& end = _pendingActors.end();
		for(; it != end; ++it)
		{
			_actors.push_back((*it));
		}
		_pendingActors.clear();
	}

	if(_pendingUIActors.size() > 0)
	{
		auto& it = _pendingUIActors.begin();
		auto& end = _pendingUIActors.end();
		for(; it != end; ++it)
		{
			_uiActors.push_back((*it));
		}
		_pendingUIActors.clear();

	}

	if(_camera)
	{
		_camera->Update(deltaTime);
	}

	//Lights are updated only for theirs transform update
	if(_ambientLight)
	{
		_ambientLight->Update();
	}

	if(_directionalLight)
	{
		_directionalLight->Update();
	}

	auto& it = _actors.begin();
	auto& end = _actors.end();
	for(; it != end; ++it)
	{
		if((*it)->IsEnabled())
		{
			(*it)->Update(deltaTime);
		}
	}

	auto& uiIT = _uiActors.begin();
	auto& uiEnd = _uiActors.end();
	for(; uiIT != uiEnd; ++uiIT)
	{
		(*uiIT)->Update(deltaTime);
	}
}

void Scene::PreSimulate()
{
	auto& it = _actors.begin();
	auto& end = _actors.end();
	for(; it != end; ++it)
	{
		if((*it)->IsEnabled() && !(*it)->IsPendingKill())
		{
			(*it)->PreSimulate();
		}
	}
}

void Scene::PostSimulate()
{
	auto& it = _actors.begin();
	auto& end = _actors.end();
	for(; it != end; ++it)
	{
		if((*it)->IsEnabled() && !(*it)->IsPendingKill())
		{
			(*it)->PostSimulate();
		}
	}
}

void Scene::Render(Graphics* graphics)
{
	graphics->BeginGeometry();
	auto& it = _actors.begin();
	auto& end = _actors.end();
	for(; it != end; ++it)
	{
		bool isEnabled = (*it)->IsEnabled();
		bool isPendingKill = (*it)->IsPendingKill();
		if((*it)->IsEnabled() && !(*it)->IsPendingKill())
		{
			(*it)->Render(*_camera, graphics, _ambientLight, _directionalLight);
		}
	}
	graphics->EndGeometry();

	graphics->BeginUI();
	auto& uiIT = _uiActors.begin();
	auto& uiEnd = _uiActors.end();
	for(; uiIT != uiEnd; ++uiIT)
	{
		(*uiIT)->Render(*_camera, graphics);
	}

	graphics->EndUI();
}
