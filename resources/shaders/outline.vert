#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTexCoord;

layout (std140, binding = 0) uniform CameraData
{
	mat4 view;
	mat4 projection;
	vec4 cameraPos;
};

uniform mat4 model;

void main()
{
	float outlineScale = 0.04;
	vec3 pos = aPos * (1.0 + outlineScale);
	gl_Position = projection * view * model * vec4(pos, 1.0);
}
