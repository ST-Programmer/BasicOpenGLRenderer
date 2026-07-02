#version 410 core

in vec3 vertexColor;
in vec3 normal;
in vec2 texCoords;
in vec3 fragPos;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec3 u_LightSourceColor;
uniform vec3 u_LightPos;
uniform mat3 u_NormalMatrix;
uniform vec3 u_CameraPos;

out vec4 fragColor;

void main()
{
	vec3 norm = normalize(u_NormalMatrix * normal);
	vec3 lightDir = normalize(u_LightPos - fragPos);

	// Ambient
	float ambientStrength = 0.20f;
	vec3 ambient = ambientStrength * u_LightSourceColor;

	// Diffuse
	float diffuseStrength = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diffuseStrength * u_LightSourceColor;

	// Specular
	float specularStrength = 0.50f;
	float shininess = 64.0f;
	vec3 viewDir = normalize(u_CameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * specularIntensity * u_LightSourceColor;

	vec3 result = texture(diffuse0, texCoords).rgb * (ambient + diffuse)
					+ texture(specular0, texCoords).rgb * specular;
	fragColor = vec4(result, 1.0f);
}
