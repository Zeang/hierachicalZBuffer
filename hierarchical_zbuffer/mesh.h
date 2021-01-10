#pragma once

#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#ifdef GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;
};

struct Triangle {
	Vertex v[3];
};