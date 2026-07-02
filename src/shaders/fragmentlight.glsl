#version 410 core

out vec4 fragColor;

uniform vec3 u_LightSourceColor;

void main()
{
	fragColor = vec4(u_LightSourceColor, 1.0f);
}
