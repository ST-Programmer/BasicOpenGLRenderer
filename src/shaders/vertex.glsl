#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

out vec3 normal;
out vec3 vertexColor;
out vec2 texCoords;
out vec3 fragPos;

void main()
{
	gl_Position = u_ViewProj * u_Model * vec4(aPos, 1.0f);
	texCoords = aTexCoords;
	normal = aNormal;
	fragPos = vec3(u_Model * vec4(aPos, 1.0f));
	vertexColor = aColor;
}
