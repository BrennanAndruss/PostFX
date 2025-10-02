#version 420 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
	vec3 posOS;
	vec3 norOS;
} gs_in[];

layout (std140, binding = 0) uniform CameraData
{
	mat4 view;
	mat4 projection;
	vec4 cameraPos;
};

uniform mat4 model;

void emitLine(vec3 a, vec3 b)
{
	gl_Position = projection * view * model * vec4(a, 1.0);
	EmitVertex();
	gl_Position = projection * view * model * vec4(b, 1.0);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	float normalLen = 0.01;
	for (int i = 0; i < 3; i++)
	{
		vec3 p = gs_in[i].posOS;
		vec3 n = normalize(gs_in[i].norOS);
		emitLine(p, p + n * normalLen);
	}
}