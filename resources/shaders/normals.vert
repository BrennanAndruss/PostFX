#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;

// Output to geometry shader in object space
out VS_OUT
{
	vec3 posOS;
	vec3 norOS;
} vs_out;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	vs_out.posOS = aPos;
	vs_out.norOS = aNor;
}