#version 330 core

in VertexData
{
	vec2 texcoords;
} iData;

uniform sampler2D diffuse;
uniform vec3 ambient;

layout (location = 0) out vec4 oDiffuseColor;

void main()
{    
	oDiffuseColor = vec4(texture(diffuse, iData.texcoords)) * vec4(ambient, 1.0f);
}