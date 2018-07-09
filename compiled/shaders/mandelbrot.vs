#version 410

layout (location = 0) in vec3 m_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = vec4(m_pos, 1.0);
}