#version 450 core

in vec3 aPos;

uniform vec4 projection;
uniform vec4 view;
uniform vec4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}