#version 330 core

in vec4 fragmentColor;
in vec4 outVertexNormal;

uniform vec4 AmbientLight;
uniform vec4 DirectionalLight;
uniform vec4 DirectionalForward;

out vec4 color;

void main()
{
	//color = vec4(abs(outVertexNormal.xyz), 1);
	float LN = max(dot(outVertexNormal, -DirectionalForward), 0.0);
	color = LN * fragmentColor * DirectionalLight + AmbientLight;
}