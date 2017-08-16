#include "Shader.h"

#include <fstream>
#include <vector>

Shader::Shader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) : _vertexShaderFilename(vertexShaderFileName), _fragmentShaderFilename(fragmentShaderFileName), _programID(0)
{

}

Shader::~Shader()
{

}

void Shader::Initialize()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(_vertexShaderFilename.c_str(), std::ios::in);
	if(vertexShaderStream.is_open())
	{
		std::string line = "";
		while(std::getline(vertexShaderStream, line))
		{
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	}
	else
	{
		printf("Cannot load vertex shader. Impossible to open %s\na", _vertexShaderFilename.c_str());
		return;
	}

	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(_fragmentShaderFilename.c_str(), std::ios::in);
	if(fragmentShaderStream.is_open())
	{
		std::string line = "";
		while(std::getline(fragmentShaderStream, line))
		{
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	}
	else
	{
		printf("Cannot load fragment shader. Impossible to open %s\n", _fragmentShaderFilename.c_str());
		return;
	}

	GLint result = GL_FALSE;
	GLint infoLogLength;

	printf("Compiling shader %s\n", _vertexShaderFilename.c_str());
	char const* vertexSourcePtr = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePtr, nullptr);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
		return;
	}

	printf("Compiling shader %s\n", _fragmentShaderFilename.c_str());
	char const* fragmentSourcePtr = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePtr, nullptr);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
		return;
	}

	printf("Linking program\n");

	_programID = glCreateProgram();
	glAttachShader(_programID, vertexShaderID);
	glAttachShader(_programID, fragmentShaderID);
	glLinkProgram(_programID);

	glGetProgramiv(_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(_programID, infoLogLength, nullptr, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
		return;
	}

	glDetachShader(_programID, vertexShaderID);
	glDetachShader(_programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::Shutdown()
{
	glDeleteProgram(_programID);
	_programID = 0;
}
