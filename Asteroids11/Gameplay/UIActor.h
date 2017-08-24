#pragma once

#include "Core/UITransform.h"
#include "Core/ResourceManager.h"
#include "Rendering/FontRenderer.h"

class Camera;
class Graphics;

class UIActor
{
protected:
	UITransform _transform;
	FontRenderer* _fontRenderer;

public:
	void Initialize(ResourceManager& resourceManager);
	void Shutdown();

	virtual void Update(float deltaTime); 
	void Render(const class Camera& camera, Graphics* graphics);

	void SetText(const std::string& text);
	void SetColor(const glm::vec4& color);
	void SetFontSize(unsigned int size);

	inline UITransform& GetTransform()
	{
		return _transform;
	}
};