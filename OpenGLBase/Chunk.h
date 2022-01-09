#pragma once

#include <glad/glad.h>

#include <vector>

#include "Voxel.h"
#include "Shader.h"

class Chunk {
public:
	Chunk();
	~Chunk();
	void CreateMesh();
	void AddVoxel(GLuint x, GLuint y, GLuint z);
	void Update(float dt);
	void Draw(Shader &shader, GLuint x, GLuint y, GLuint z);
	static constexpr GLint CHUNK_SIZE = 128;

private:
	Voxel ***m_pVoxels;

	std::vector<GLfloat> m_vertices;
	std::vector<unsigned int> m_normals;
	GLuint m_chunkVertexBuffer{0}, m_chunkNormalBuffer{0}, m_chunkVAO{0};
};