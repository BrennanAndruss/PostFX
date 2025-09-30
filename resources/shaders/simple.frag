#version 420 core

#define MAX_LIGHTS 16

in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTexCoord;

out vec4 fragColor;

struct Light {
	vec4 color_intensity;	// rgb = color, a = intensity
	vec4 position_range;	// xyz = position, w = range
	vec4 direction_type;	// xyz = direction, w = type
};

layout (std140, binding = 0) uniform CameraData
{
	mat4 view;
	mat4 projection;
	vec4 cameraPos;
};

layout (std140, binding = 1) uniform LightData
{
	Light lights[MAX_LIGHTS];
};
uniform int numLights;

uniform vec3 matAmb;
uniform vec3 matDif;
uniform vec3 matSpec;
uniform float matShine;

void main()
{
	vec3 normal = normalize(fragNor);
	vec3 view = normalize(cameraPos.xyz - fragPos);

	vec3 ambient = matAmb;
	vec3 diffuseSum = vec3(0.0);
	vec3 specularSum = vec3(0.0);

	for (int i = 0; i < numLights; i++)
	{
		vec3 lightDir;
		float attenuation = 1.0;
		vec3 lightColor = lights[i].color_intensity.rgb * lights[i].color_intensity.a;

		if (lights[i].direction_type.w == 0) // Directional Light
		{
			lightDir = normalize(-lights[i].direction_type.xyz);
		}
		else if (lights[i].direction_type.w == 1) // Point Light
		{
			vec3 toLight = lights[i].position_range.xyz - fragPos;
			float dist = length(toLight);
			lightDir = normalize(toLight);
			attenuation = 1.0 / (dist * dist);
		}

		// Diffuse
		float dC = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = matDif * dC * lightColor;

		// Specular
		vec3 halfDir = normalize(lightDir + view);
		float sC = max(dot(normal, halfDir), 0.0);
		vec3 specular = matSpec * pow(sC, matShine) * lightColor;

		diffuseSum += diffuse * attenuation;
		specularSum += specular * attenuation;
	}

	vec3 reflection = ambient + diffuseSum + specularSum;
	fragColor = vec4(reflection, 1.0);
}
