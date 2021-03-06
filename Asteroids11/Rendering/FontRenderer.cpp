#include "FontRenderer.h"

#include "Graphics.h"
#include "Shader.h"
#include "Core/ResourceManager.h"
#include "Core/UITransform.h"
#include "Gameplay/Camera.h"

#pragma comment(lib, "freetype271.lib")

void FontRenderer::InitializeCharacterSet()
{
	for(GLubyte c = 0; c < 128; ++c)
	{
		if(_characterSet.find(c) != _characterSet.end())
		{
			glDeleteTextures(1, &_characterSet[c].TextureID);
			_characterSet[c].TextureID = 0;
			_characterSet.erase(_characterSet.find(c));
		}

		if(FT_Load_Char(_fontFace, c, FT_LOAD_RENDER))
		{
			printf("Error loading character: %c", c);
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,	//Cause generated bitmap is 8-bit grayscale
			_fontFace->glyph->bitmap.width,
			_fontFace->glyph->bitmap.rows,
			0,
			GL_RED,	//Cause generated bitmap is 8-bit grayscale
			GL_UNSIGNED_BYTE,
			_fontFace->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character;
		character.TextureID = texture;
		character.Size = glm::ivec2(_fontFace->glyph->bitmap.width, _fontFace->glyph->bitmap.rows);
		character.Bearing = glm::ivec2(_fontFace->glyph->bitmap_left, _fontFace->glyph->bitmap_top);
		character.Advance = _fontFace->glyph->advance.x;
		_characterSet.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FontRenderer::Initialize(ResourceManager& resourceManager, const std::string& fontName)
{
	if(FT_Init_FreeType(&_fontLibrary))
	{
		printf("Error initializing FT_Library\n");
		return;
	}

	if(FT_New_Face(_fontLibrary, fontName.c_str(), 0, &_fontFace))
	{
		printf("Error loading font\n");
		return;
	}

	SetFontSize(36);

	_textColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	_fontShader = resourceManager.GetShader("Shaders/fontVS.glsl", "Shaders/fontFS.glsl");

	_text = "";

	glGenBuffers(1, &_vertexBufferIndex);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
}

void FontRenderer::Shutdown()
{
	FT_Done_Face(_fontFace);
	FT_Done_FreeType(_fontLibrary);
}

void FontRenderer::Render(const Camera& camera, Graphics* graphics, const UITransform& uiTransform)
{
	graphics->SetShader(_fontShader->GetProgramID());
	graphics->DrawText(camera, _text, _characterSet, uiTransform, _textColor, _fontSize, _vertexBufferIndex);
}
