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

class FontRenderer
{
protected:
	FT_Library _fontLibrary;
	FT_Face _fontFace;
	class Shader* _fontShader;

	GLuint _vertexBufferIndex;
	glm::vec4 _textColor;
	GLuint _fontSize;

	std::string _text;

	std::map<GLuint, Character> _characterSet;

protected:
	void InitializeCharacterSet();

public:
	void Initialize(class ResourceManager& resourceManager, const std::string& fontName);
	void Shutdown();

	void Render(const class Camera& camera, class Graphics* graphics, const class UITransform& uiTransform);

	inline void SetFontSize(unsigned int fontSize)
	{
		_fontSize = fontSize;
		FT_Set_Pixel_Sizes(_fontFace, 0, fontSize);
		InitializeCharacterSet();
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