#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in int group;

out vec2 UV;

uniform mat4 ortho;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	UV = texCoord;
}
