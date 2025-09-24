#version 420 core

in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D Texture0;

void main()
{
//	fragColor = vec4(fragTexCoord.xy, 0.0, 1.0);
	fragColor = texture(Texture0, fragTexCoord);
}