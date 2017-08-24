#pragma once

#include <map>
#include <string>

#include "FreeType/ft2build.h"
#include FT_FREETYPE_H
#include "glm/glm.hpp"
#include "GL/glew.h"

struct Character
{
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class FontHelper
{
public:
	static std::map<GLuint, Character> CharacterSet;

public:
	static void Initialize(FT_Face& fontFace);
};

class FontRenderer
{
protected:
	FT_Library _fontLibrary;
	FT_Face _fontFace;
	class Shader* _fontShader;

	GLuint _vertexBufferIndex;
	glm::vec4 _textColor;

	std::string _text;

public:
	void Initialize(class ResourceManager& resourceManager, const std::string& fontName);
	void Shutdown();

	void Render(const class Camera& camera, class Graphics* graphics, const class UITransform& uiTransform);

	inline void SetFontSize(unsigned int fontSize)
	{
		FT_Set_Pixel_Sizes(_fontFace, 0, fontSize);
		FontHelper::Initialize(_fontFace);
	}

	inline void SetText(const std::string& text)
	{
		_text = text;
	}

	inline void SetColor(const glm::vec4& color)
	{
		_textColor = color;
	}
};