#version 330 core

layout(location = 0) in vec3 vertexPosition_modelSpace;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform vec4 Color;

out vec4 fragmentColor;
out vec4 outVertexNormal;

void main()
{
	gl_Position = MVP * vec4(vertexPosition_modelSpace, 1);
	fragmentColor = Color;
	outVertexNormal = normalize(ModelMatrix * vec4(vertexNormal, 0));
}