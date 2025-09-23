#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec3 fragNor;
out vec2 fragTexCoord;

layout (std140, binding = 0) uniform CameraData
{
	mat4 view;
	mat4 projection;
	vec4 cameraPos;
};

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
	fragPos = aPos;
	fragNor = aNor;
	fragTexCoord = aTexCoord;
}
