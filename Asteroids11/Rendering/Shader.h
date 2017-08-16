#pragma once

#include <string>
#include "GL/glew.h"

class Shader
{
protected:
	std::string _vertexShaderFilename;
	std::string _fragmentShaderFilename;
	GLuint _programID;

public:
	Shader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
	~Shader();

	void Initialize();
	void Shutdown();

	inline GLuint GetProgramID() const
	{
		return _programID;
	}

	inline const std::string& GetVertexShaderFilename() const
	{
		return _vertexShaderFilename;
	}
	
	inline const std::string& GetFragmentShaderFilename() const
	{
		return _fragmentShaderFilename;
	}
};

