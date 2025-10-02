#version 420 core

#define MAX_LIGHTS 16

in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTexCoord;

out vec4 fragColor;

struct Light
{
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

float stepBand(float x, float bands)
{
	return floor(x * bands) / bands;
}

void main()
{
	float bands = 4.0;

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

		// Quantize diffuse per-light
		float dC = max(dot(normal, lightDir), 0.0);
		float dQ = floor(dC * bands) / bands;
		diffuseSum += matDif * dQ * lightColor * attenuation;

		// Threshold specular highlights
		vec3 halfDir = normalize(lightDir + view);
		float sC = pow(max(dot(normal, halfDir), 0.0), matShine);
		float sQ = step(0.5, sC);
		specularSum += matSpec * sQ * lightColor * attenuation;
	}

	// Rim lighting
	float rim = pow(1.0 - max(dot(normal, view), 0.0), 2.0);
	float rimQ = step(0.6, rim) * 0.25;

	vec3 reflection = ambient + diffuseSum + specularSum + vec3(rimQ);
	fragColor = vec4(reflection, 1.0);
}
