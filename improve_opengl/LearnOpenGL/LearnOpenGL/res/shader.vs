
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);
	vertexColor = aColor;
	texCoord = aTexCoord;
}
