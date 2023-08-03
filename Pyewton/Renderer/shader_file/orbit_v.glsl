#version 450 core
in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
}
