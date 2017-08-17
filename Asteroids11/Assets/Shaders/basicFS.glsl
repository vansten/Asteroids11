#version 330 core

in vec4 fragmentColor;
in vec4 outVertexNormal;

out vec4 color;

void main()
{
	color = vec4(abs(outVertexNormal.xyz), 1);
}