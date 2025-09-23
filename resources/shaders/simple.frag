#version 420 core

in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTexCoord;

out vec4 fragColor;

layout (std140, binding = 0) uniform CameraData
{
	mat4 view;
	mat4 projection;
	vec4 cameraPos;
};

layout (std140, binding = 1) uniform LightData
{
	vec3 lightDir;
};

uniform vec3 matAmb;
uniform vec3 matDif;
uniform vec3 matSpec;
uniform float matShine;

void main()
{
	vec3 normal = normalize(fragNor);
	vec3 light = normalize(-lightDir);

	// Ambient lighting
	vec3 ambient = matAmb;

	// Diffuse reflection
	float dC = max(dot(normal, light), 0.0);
	vec3 diffuse = matDif * dC;

	// Specular reflection
	vec3 view = normalize(cameraPos.xyz - fragPos);
	vec3 halfway = normalize(light + view);
	float sC = max(dot(normal, halfway), 0.0);
	vec3 specular = matSpec * pow(sC, matShine);

	vec3 reflection = ambient + diffuse + specular;
	fragColor = vec4(reflection, 1.0);

	// debug
	// fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
