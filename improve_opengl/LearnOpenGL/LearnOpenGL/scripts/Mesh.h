#pragma once

#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// ��������
	std::vector<Vertex> vertices;
	// ��������
	std::vector<unsigned int> indices;
	// ͼƬ
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	void Draw(Shader &shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif