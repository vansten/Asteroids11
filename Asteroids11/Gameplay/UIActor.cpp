#include "UIActor.h"

#include "Camera.h"
#include "Rendering/Graphics.h"

#include "Core/Input.h"
#include "Core/Engine.h"

void UIActor::Initialize(ResourceManager& resourceManager)
{
	_fontRenderer = NewObject(FontRenderer);
	_fontRenderer->Initialize(resourceManager, "Assets/Fonts/Arial.ttf");
}

void UIActor::Shutdown()
{
	if(_fontRenderer)
	{
		Memory::GetInstance()->Deallocate<FontRenderer>(_fontRenderer);
		_fontRenderer = nullptr;
	}
}

void UIActor::Update(float deltaTime)
{
}

void UIActor::Render(const Camera& camera, Graphics* graphics)
{
	if(_fontRenderer)
	{
		_fontRenderer->Render(camera, graphics, _transform);
	}
}

void UIActor::SetText(const std::string& text)
{
	if(_fontRenderer)
	{
		_fontRenderer->SetText(text);
	}
}

void UIActor::SetColor(const glm::vec4& color)
{
	if(_fontRenderer)
	{
		_fontRenderer->SetColor(color);
	}
}

void UIActor::SetFontSize(unsigned int size)
{
	if(_fontRenderer)
	{
		_fontRenderer->SetFontSize(size);
	}
}
