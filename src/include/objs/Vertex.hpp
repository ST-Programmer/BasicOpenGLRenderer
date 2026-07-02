#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 uv;

	static void AddCubeFace(std::vector<Vertex> &vertices, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D) // CCW/Consistent Winding
	{
		glm::vec3 firstEdge = B - A;
		glm::vec3 secondEdge = C - B;
		glm::vec3 normal = glm::normalize(glm::cross(firstEdge, secondEdge));

		vertices.push_back({A, normal, glm::vec3(1.0f, 0.5f, 0.31f), glm::vec2(0.0f, 0.0f)});
		vertices.push_back({B, normal, glm::vec3(1.0f, 0.5f, 0.31f), glm::vec2(1.0f, 0.0f)});
		vertices.push_back({C, normal, glm::vec3(1.0f, 0.5f, 0.31f), glm::vec2(1.0f, 1.0f)});
		vertices.push_back({D, normal, glm::vec3(1.0f, 0.5f, 0.31f), glm::vec2(0.0f, 1.0f)});
	}
};
